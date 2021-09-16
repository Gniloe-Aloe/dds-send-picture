
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <list>
#include <deque>
#include <queue>

/* Include the C++ DDS API. */
#include "dds/dds.hpp"

/* Include data type and specific traits to be used with the C++ DDS API. */
#include "HelloWorldData.hpp"
#include "picture.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include "base64.hpp"
#include "timer.hpp"


void print_pixels_chanell_matrix(uint8_t* point_to_pixel_matrix, const unsigned& width, const unsigned& heigh) {
    for (int h = 0; h < heigh; ++h) {
        for (int w = 0; w < width * 3; ++w) {
            std::cout << (int)point_to_pixel_matrix[width * 3 * h + w] << " ;";
        }
        std::cout << '\n';
    }
}


void thread_decoding(std::queue<std::string>& messages, bool& flag) {
    while (flag != false) {
        while (!messages.empty()) {
            std::string dec_jpg = base64_decode(messages.front());
            std::vector<uchar> data(dec_jpg.begin(), dec_jpg.end());
            cv::Mat base64_image = cv::imdecode(cv::Mat(data), 1);
            //cv::imwrite("/home/dkosinov/win_home/Desktop/pic/test.png", base64_image);
            messages.pop();
            std::cout << "Width = " << base64_image.cols << '\t' << "Heigh = " << base64_image.rows << '\n';
        }
    }
}
void test_thread() {
    std::cout << "test thread start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "test thread stop" << std::endl;
}

int main() {
    
    try {
        std::cout << "=== [Subscriber] Create reader." << std::endl;

        /* First, a domain participant is needed.
         * Create one on the default domain. */
        dds::domain::DomainParticipant participant(org::eclipse::cyclonedds::domain::default_id());

        /* To subscribe to something, a topic is needed. */
        //dds::topic::Topic<HelloWorldData::Msg> topic(participant, "HelloWorldData_Msg");
        dds::topic::Topic<HelloWorldData::Msg> topic(participant, "any_topic_name");
        /* A reader also needs a subscriber. */
        dds::sub::Subscriber subscriber(participant);

        /* Now, the reader can be created to subscribe to a HelloWorld message. */
        dds::sub::DataReader<HelloWorldData::Msg> reader(subscriber, topic);

        /* Poll until a message has been read.
         * It isn't really recommended to do this kind wait in a polling loop.
         * It's done here just to illustrate the easiest way to get data.
         * Please take a look at Listeners and WaitSets for much better
         * solutions, albeit somewhat more elaborate ones. */
        std::cout << "=== [Subscriber] Wait for message." << std::endl;
        timer t;
        bool poll = true;
        

        //очередь сообщений, которые в отдельном потоке декодируются в изображения
        std::queue<std::string> messages_queue;
        //std::list<cv::Mat> pictures;

        std::thread decoding_thread([&messages_queue, &poll] {thread_decoding(messages_queue, poll); });

        

        
        while (poll) {
            /* For this example, the reader will return a set of messages (aka
             * Samples). There are other ways of getting samples from reader.
             * See the various read() and take() functions that are present. */
            dds::sub::LoanedSamples<HelloWorldData::Msg> samples;

            /* Try taking samples from the reader. */
            samples = reader.take();

            /* Are samples read? */
            if (samples.length() > 0) {
                /* Use an iterator to run over the set of samples. */
                dds::sub::LoanedSamples<HelloWorldData::Msg>::const_iterator sample_iter;
                for (sample_iter = samples.begin(); sample_iter < samples.end();++sample_iter) {
                    /* Get the message and sample information. */
                    const HelloWorldData::Msg& msg = sample_iter->data();
                    const dds::sub::SampleInfo& info = sample_iter->info();

                    /* Sometimes a sample is read, only to indicate a data
                     * state change (which can be found in the info). If
                     * that's the case, only the key value of the sample
                     * is set. The other data parts are not.
                     * Check if this sample has valid data. */
                    if (info.valid()) {
                        //std::cout << "=== [Subscriber] Message received:" << std::endl;

                        // декодируем полученное сообщение
                        /*std::string dec_jpg = base64_decode(msg.message());
                        std::vector<uchar> data(dec_jpg.begin(), dec_jpg.end());
                        cv::Mat base64_image = cv::imdecode(cv::Mat(data), 1);*/

                        messages_queue.push(msg.message());

                        // сохраняем изображение
                        // std::cout << "Picture id: " << msg.picture_width() << '\t' << "Width = " << base64_image.cols << '\t' << "Heigh = " << base64_image.rows << '\n';
                        //std::cout << "Picture id: " << msg.picture_width() << '\t' << "Width = " << "1920" << '\t' << "Heigh = " << "1080" << '\n';
                        //cv::imwrite("/home/dkosinov/win_home/Desktop/pic/test-" + std::to_string(msg.picture_width()) + ".png", base64_image);

                        // выход из цикла, если мы получили десятое сообщение
                        if (msg.picture_width() == 24) poll = false;
                    }
                }
            }
            else {
                //std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
        // дожидаемся, пока все изображения будут декодированы
        std::cout << "=== [Subscriber] Wait decoding all messages." << std::endl;
        decoding_thread.join();
    }
    catch (const dds::core::Exception& e) {
        std::cerr << "=== [Subscriber] DDS exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        std::cerr << "=== [Subscriber] C++ exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    
    std::cout << "=== [Subscriber] Done." << std::endl;

    return EXIT_SUCCESS;
}