#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
//my timer
#include "timer.hpp"
/* Include the C++ DDS API. */
#include "dds/dds.hpp"

/* Include data type and specific traits to be used with the C++ DDS API. */
#include "HelloWorldData.hpp"

#include "picture.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include "base64.hpp"

int main() {

    timer t;

    try {
        std::cout << "=== [Publisher] Create writer." << std::endl;

        /* First, a domain participant is needed.
         * Create one on the default domain. */
        dds::domain::DomainParticipant participant(org::eclipse::cyclonedds::domain::default_id());
        /* To publish something, a topic is needed. */
        
        dds::topic::Topic<HelloWorldData::Msg> topic(participant, "any_topic_name");

        /* A writer also needs a publisher. */
        dds::pub::Publisher publisher(participant);

        /* Now, the writer can be created to publish a HelloWorld message. */
        dds::pub::DataWriter<HelloWorldData::Msg> writer(publisher, topic);

        /* For this example, we'd like to have a subscriber to actually read
         * our message. This is not always necessary. Also, the way it is
         * done here is just to illustrate the easiest way to do so. It isn't
         * really recommended to do a wait in a polling loop, however.
         * Please take a look at Listeners and WaitSets for much better
         * solutions, albeit somewhat more elaborate ones. */

        std::cout << "=== [Publisher] Waiting for subscriber." << std::endl;

        while (writer.publication_matched_status().current_count() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        //создаём изображение для отправки
        cv::Mat image = cv::imread("/home/dkosinov/win_home/Desktop/2560-1440.png");
        //cv::Mat image = cv::imread("/home/dkosinov/win_home/Desktop/new_my.png");
        //cv::Mat image = cv::imread("/home/dkosinov/win_home/Desktop/colour.jpg");
        //cv::Mat image = cv::imread("/home/dkosinov/win_home/Desktop/size.png");
        //cv::Mat image = cv::imread("/home/dkosinov/win_home/Desktop/1920-1080.png");
        //cv::Mat image = cv::imread("/home/dkosinov/win_home/Desktop/little.png");

        //кодируем изображение в base64
        std::vector<uchar> buffer;
        buffer.resize(static_cast<size_t>(image.rows) * static_cast<size_t>(image.cols));
        cv::imencode(".png", image, buffer);
        std::string encoding = base64_encode(buffer.data(), buffer.size());

        for (int i = 0; i < 25; ++i) {

            //кодируем изображение в цикле
            /*std::vector<uchar> buffer;
            buffer.resize(static_cast<size_t>(image.rows) * static_cast<size_t>(image.cols));
            cv::imencode(".png", image, buffer);
            std::string encoding = base64_encode(buffer.data(), buffer.size());*/

            /* Create a message to write. */

            HelloWorldData::Msg msg(i, encoding);

            /* Write the message. */
            std::cout << "=== [Publisher] Write sample." << std::endl;
            writer.write(msg);

            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            /* With a normal configuration (see dds::pub::qos::DataWriterQos
             * for various different writer configurations), deleting a writer will
             * dispose all its related message.
             * Wait for the subscriber to have stopped to be sure it received the
             * message. Again, not normally necessary and not recommended to do
             * this in a polling loop. */
            
           // std::cout << "writer.publication_matched_status().current_count() = " << writer.publication_matched_status().current_count() << '\n';
           
        }
        std::cout << "=== [Publisher] Waiting for sample to be accepted." << std::endl;
        while (writer.publication_matched_status().current_count() > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    catch (const dds::core::Exception& e) {
        std::cerr << "=== [Publisher] Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        std::cerr << "=== [READER] C++ exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Publisher] Done." << std::endl;

    return EXIT_SUCCESS;
}