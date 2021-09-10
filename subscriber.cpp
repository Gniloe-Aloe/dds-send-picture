
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

//#include <opencv2/highgui.hpp>
//#include <iostream>
//
//int main(int argc, char** argv) {
//
//    cv::Mat image;
//    //CV_LOAD_IMAGE_COLOR
//    image = cv::imread("lena.png");
//
//    if (!image.data) {
//        std::cout << "Could not open or find the image" << std::endl;
//        return -1;
//    }
//
//    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
//    cv::imshow("Display window", image);
//
//    cv::waitKey(0);
//    return 2;
//}

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
        
        unsigned seconds_waiting_message = 0;


        

        //we expect 1 message, when we accept it - we end
        bool message_accept = 0;

        while (!message_accept) {
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
                        
                        // создаём наш класс изображения
                        Picture pic(msg.picture_width(), msg.message());
                        
                        // переносим изображение из класса в объект cv::Mat
                        pic.prepare_opencv_Mat();

                        // создаём изображение для увеличения масштаба
                        /*cv::Mat scale_image;
                        cv::resize(pic.opencv_picture, scale_image, cv::Size(500, 500));*/

                        std::cout << "Width = " << pic.opencv_picture.cols << '\t' << "Heigh = " << pic.opencv_picture.rows << '\n';

                        pic.opencv_picture;

                        std::cout << "=== [READER] Picture saved" << '\n';
                        //cv::imwrite("/home/dkosinov/Desktop/opencv-test/new_my.png", pic.opencv_picture);
                        cv::imwrite("/home/dkosinov/win_home/Desktop/new_my.png", pic.opencv_picture);


                        /*cv::imshow("read image", pic.opencv_picture);
                        cv::waitKey(0);*/



                        std::cout << "=== [READER] Picture end received" << '\n';
                        
                        /*std::string image_path = cv::samples::findFile("/home/dkosinov/Desktop/opencv-test/lena.png");
                        image_vector[i] = cv::imread(image_path);*/
                        
                        //тестово выводим картинку

                        //std::string image_path = cv::samples::findFile("/home/dkosinov/Desktop/opencv-test/test-image.png");
                        
                        //image = cv::imread(image_path);

                        //сделать картинку чёрно-белой
                        //cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
                        
                        //обрисовать границы
                        //cv::Canny(image, canny_image, 50, 150);

                        //cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
                        //cv::imshow("gray!", gray_image);

                        //утолщение границ
                        /*cv::Mat kernel_image = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
                        cv::dilate(canny_image, image, kernel_image);*/
                        

                        //scale - size без параметров
                        //cv::resize(image, resize_image, cv::Size(), 0.2, 0.2);

                        //обрезать картинку с помощью прямоугольника
                       /* cv::Rect crop_rect(20, 20, 200, 200);
                        crop_image = image(crop_rect);*/

                        
                        /*uint8_t pix = image.at<uint8_t>(0, 0);
                        std::cout << int(pix-48) << std::endl;*/


                        //we break this cycle
                        message_accept = true;
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
