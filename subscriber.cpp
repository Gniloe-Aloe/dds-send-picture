
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

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
            std::cout << (int)point_to_pixel_matrix[width * 3 * h + w] <<" ;";
        }
        std::cout << '\n';
    }

}


int main() {
    
    try {

        std::cout << "=== [READER] Create reader." << std::endl;
        
        /* First, a domain participant is needed.
        Create one on the default domain.*/

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
        std::cout << "=== [READER] Wait for message." << std::endl;
        
        //we expect 1 message, when we accept it - we end
        unsigned message_accept = 0;

        timer t;
        //!message_accept
        while (message_accept != 9) {
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
                for (sample_iter = samples.begin(); sample_iter < samples.end(); ++sample_iter) {
                    /* Get the message and sample information. */
                    const HelloWorldData::Msg& msg = sample_iter->data();
                    const dds::sub::SampleInfo& info = sample_iter->info();
                    
                    /* Sometimes a sample is read, only to indicate a data
                     * state change (which can be found in the info). If
                     * that's the case, only the key value of the sample
                     * is set. The other data parts are not.
                     * Check if this sample has valid data. */
                    if (info.valid()) {
                        std::cout << "=== [READER] Picture received:" << std::endl;
                        
                       
                        // раскодируем
                        std::string dec_jpg = base64_decode(msg.message());
                        std::vector<uchar> data(dec_jpg.begin(), dec_jpg.end());
                        cv::Mat base64_image = cv::imdecode(cv::Mat(data), 1);

                       
                        
                        // сохраняем base64 картинку на рабочий стол винды
                        
                        std::cout << "=== [READER] Picture saved" << '\n';
                        //cv::imwrite("/home/dkosinov/win_home/Desktop/pic/base64-picture-" + std::to_string(msg.picture_width()) + ".png", base64_image);
                        
                       

                        // Выводим картинку. Увеличиваем её масштаб, если она слишком маленькая (пока не выводим)
                        /*if (base64_image.cols < 80 || base64_image.rows < 80) {
                            cv::Mat scale_image;
                            cv::resize(base64_image, scale_image, cv::Size(), 10., 10.);
                            cv::imshow("Scaled picture", scale_image);
                            cv::waitKey(0);
                        }
                        else {
                            cv::imshow("Picture", base64_image);
                            cv::waitKey(0);
                        }*/

                        std::cout << "Width = " << base64_image.cols << '\t' << "Heigh = " << base64_image.rows << '\n';

                        std::cout << "=== [READER] Picture end received" << '\n';

                        //we break this cycle
                        message_accept = msg.picture_width();
                    }
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
    } catch (const dds::core::Exception& e) {
        std::cerr << "=== [READER] DDS exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "=== [READER] C++ exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [READER] Done." << std::endl;

    return EXIT_SUCCESS;
}
