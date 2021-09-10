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


//using namespace org::eclipse::cyclonedds;

int main() {    
    timer timer;
    
    try {
        std::cout << "=== [Publisher] Create writer." << std::endl;

        //First, a domain participant is needed.
        //Create one on the default domain.
        dds::domain::DomainParticipant participant(org::eclipse::cyclonedds::domain::default_id());

        /* To publish something, a topic is needed. */
        //dds::topic::Topic<HelloWorldData::Msg> topic(participant, "HelloWorldData_Msg");
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

        // создаём изображение, которое будем отправлять
        cv::Mat image;
        image = cv::imread("/home/dkosinov/Desktop/opencv-test/test1920-1080.jpg");

        //cv::Mat scale_image;

        //cv::resize(image, scale_image, cv::Size(500, 500));

        //cv::imshow("Send image", scale_image);b

        std::cout << "width = " << image.cols << '\t' << "heigh = " << image.rows << '\n';

        //cv::waitKey(0);

        Picture pic(image);

        //Write the message from my picture class
        HelloWorldData::Msg msg(pic.get_picture_width(), pic.get_picture_message() );
       
            
        std::cout << "=== [Publisher] Picture send" << std::endl;
        
        
        writer.write(msg);
       
        
        /* With a normal configuration (see dds::pub::qos::DataWriterQos
         * for various different writer configurations), deleting a writer will
         * dispose all its related message.
         * Wait for the subscriber to have stopped to be sure it received the
         * message. Again, not normally necessary and not recommended to do
         * this in a polling loop. */

        std::cout << "=== [Publisher] Waiting for sample to be accepted." << std::endl;

        // перестал работать
       /* while (writer.publication_matched_status().current_count() > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }*/
    }
    catch (const dds::core::Exception& e) {
        std::cerr << "=== [Publisher] exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        std::cerr << "=== [Publisher] C++ exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Publisher] Done." << std::endl;

    return EXIT_SUCCESS;
}