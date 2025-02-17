
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include <windows.h>
#include "../ConsoleUI/console_ui.h"
#include <tchar.h>
#include"../ConsoleColors/console_colors.h"
#include<vector>
#include<sstream>
#include<string>


LPCTSTR GetCurrentFile()
{
#ifdef _UNICODE
    return __FILEW__;
#else
    return __FILE__;
#endif // _UNICODE  
}

const int count_to_ignore = 8;

void getPathToCurrentFolder(int size_of_input, int count_to_ignore, LPTSTR result)
{
    LPCTSTR input = GetCurrentFile();

    for (int i = 0; i < (size_of_input - count_to_ignore); i++)
    {
        result[i] = input[i];
    }
}

LPCTSTR yes = TEXT("YyÍí");
LPCTSTR no = TEXT("NnÒò");

auto Str_To_Bool = [](LPTSTR result, LPCTSTR error, int& error_code)->bool
    {
        size_t length = _tcslen(yes);
        
        for (size_t i = 0; i < length; i++)
        {
            if ((int)yes[i] == (int)(*result))
            {
                error_code = 0;               
                return true;
            }
        }

        for (size_t i = 0; i < length; i++)
        {
            if ((int)no[i] == (int)(*result))
            {
                error_code = 0;                
                return false;
            }
        }

        error_code = -1;
        error = TEXT("Unable to convert to bool!");
        return false;
    };

auto Str_to_int = [](LPTSTR result, LPCTSTR error, int& error_code) -> int
    {
        int temp = INT32_MIN;
        error_code = 0;

#ifdef _UNICODE
        temp = _wtoi(result);
#else
        temp = atoi(result);
#endif // _UNICODE 

        if (temp == INT32_MIN)
        {
            error = TEXT("Unable to convert to int!");
            error_code = -1;
        }
            
        return temp;
    };

auto positive_double_validator = [](double result, LPCTSTR error)->bool
    {
        if (result <= 0)
        {
            error = TEXT("Value can't ne zero or negative!");
            return false;
        }

        return true;
    };

auto Str_to_double = [](LPTSTR result, LPCTSTR error, int& error_code) -> double
    {
        double temp = DBL_MIN;
        error_code = 0;
        
#ifdef _UNICODE
        LPTSTR endPtr;
        temp = _tcstod(result, &endPtr);
#else
        char* endptr;
        temp = strtod(result, &endptr);
#endif // _UNICODE 

        if (temp == DBL_MIN)
        {
            error = TEXT("Unable to convert to int!");
            error_code = -1;
        }

        return temp;
    };

auto int_positive_Validator_0 = [](int result, LPCTSTR error) -> bool
    {
        if (result < 0)
        {
            error = TEXT("Value can't be negative!");
            return false;
        }

        return true;
    };

LPCTSTR smooth_msg = TEXT("Please choose smoothing algorithm:\n\tPress 1 - Summation without Scaling.\n\tPress 2 - Summation with scaling.\n\tPress 3 - image convolution with a Gaussian kernel\n\tPress 4 - Finding the average value in the neighborhood\n\t Press 5 - Double Filtration");

const char* originNameWindow = "Original";

int main()
{
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);  

    printf("Lab work 1\n");

    printf("Program starting...\n");

    HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);

    std::shared_ptr<io::ConsoleInputOutput> consoleIOPtr = std::make_shared<io::ConsoleInputOutput>(handler);    
            
    //Main Programm Cycle

    bool img_opened = false;

    std::vector<IplImage*> Images;

    IplImage* srcImg;

    for (; ;)
    {    
        std::string path;

        consoleIOPtr->PrintLine(TEXT("Enter path to file:"));

        getline(std::cin, path);
       
        srcImg = cvLoadImage(path.c_str(), CV_LOAD_IMAGE_UNCHANGED);
                
        if (srcImg)
        {
            cvNamedWindow(originNameWindow, CV_WINDOW_AUTOSIZE);

            cvShowImage(originNameWindow, srcImg);

            cv::waitKey(0);

            img_opened = true;
        }
        else
        {
            consoleIOPtr->PrintLine("Unable to open file!", Colors::RED, Colors::BLACKBack);
            img_opened = false;
        }     

        if (img_opened)
        {
            bool need_smooth = consoleIOPtr->Input<bool>(TEXT("Would you like to smooth your image? Y | N"), Str_To_Bool);

            bool tryMoreSmooth = false;

            if (need_smooth)
            {
                do
                {
                    IplImage* destImg = cvCloneImage(srcImg);

                    Images.push_back(destImg);

                    const char* windowName = "";

                    int smoothType = 0;

                    int comand_numb = consoleIOPtr->Input<int>(smooth_msg, Str_to_int, [](int result, LPCTSTR error) -> bool
                        {
                            if (result <= 0 && result > 5)
                            {
                                error = TEXT("Incorrect range of the command!");
                                return false;
                            }

                            return true;
                        });

                    switch (comand_numb)
                    {
                    case 1:    
                        consoleIOPtr->PrintLine(TEXT("Summation without Scaling"));
                        smoothType = CV_BLUR_NO_SCALE;
                        windowName = "Summation without Scaling";
                        break;
                    case 2:
                        consoleIOPtr->PrintLine(TEXT("Summation with scaling"));
                        smoothType = CV_BLUR;
                        windowName = "Summation with scaling";
                        break;
                    case 3:
                        consoleIOPtr->PrintLine(TEXT("image convolution with a Gaussian kernel"));
                        smoothType = CV_GAUSSIAN;
                        windowName = "image convolution with a Gaussian kernel";
                        break;
                    case 4:
                        consoleIOPtr->PrintLine(TEXT("Finding the average value in the neighborhood"));
                        smoothType = CV_MEDIAN;
                        windowName = "Finding the average value in the neighborhood";
                        break;
                    case 5:
                        consoleIOPtr->PrintLine(TEXT("Double Filtration"));
                        smoothType = CV_BILATERAL;
                        windowName = "Double Filtration";
                        break;                    
                    }

                    int size1 = consoleIOPtr->Input<int>(TEXT("Enter the value of size1 parameter."), Str_to_int);

                    int size2 = consoleIOPtr->Input<int>(TEXT("Enter the value of size2 parameter."), Str_to_int);

                    int sigma1 = consoleIOPtr->Input<int>(TEXT("Enter the value of sigma1 parameter."), Str_to_double);

                    int sigma2 = consoleIOPtr->Input<int>(TEXT("Enter the value of sigma2 parameter."), Str_to_double);

                    cvSmooth(srcImg, destImg, smoothType, size1, size2, sigma1, sigma2);
                   
                    cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

                    cvShowImage(windowName, destImg);

                    cv::waitKey(0);

                    tryMoreSmooth = consoleIOPtr->Input<bool>(TEXT("Would you like to try another smooth method? Y | N"), Str_To_Bool);

                    cvDestroyWindow(windowName);

                } while (tryMoreSmooth);
            }

            bool need_resize = consoleIOPtr->Input<bool>(TEXT("Would you like to resize your image? Y | N"), Str_To_Bool);

            if (need_resize)
            {
                IplImage* last;

                bool more_resize = false;
                
                do
                {
                    double w_multipl = consoleIOPtr->Input<double>(TEXT("Enter width multiplyer."), Str_to_double, positive_double_validator);

                    double h_multipl = consoleIOPtr->Input<double>(TEXT("Enter height multiplyer."), Str_to_double, positive_double_validator);

                    int interpolation = consoleIOPtr->Input<int>(TEXT("Choose interpolation method: Press 1 - nearest-neigbor\n\t Press 2 - Bilinear interpolation\n\t Press 3 - Cubic interpolation.\n\tPress 4 - BiCubic interpolation"),
                        Str_to_int, [](int result, LPCTSTR error)-> bool
                        {
                            if (result <= 0 || result > 4)
                            {
                                error = TEXT("Incorrect input Range!");
                                return false;
                            }

                            return true;
                        });

                    last = Images[Images.size() - 1];

                    std::stringstream strStream;

                    strStream << "Resize" << "[" << w_multipl << "," << h_multipl << "]";

                    char buff[20];
                    strStream.getline(buff, sizeof(buff));

                    cvNamedWindow(buff, CV_WINDOW_AUTOSIZE);
                   
                    IplImage* resizedImg = cvCreateImage(cvSize(last->width/w_multipl, last->height/h_multipl), last->depth, last->nChannels);

                    cvResize(last, resizedImg, interpolation);

                    Images.push_back(resizedImg);

                    cvShowImage(buff, resizedImg);

                    cv::waitKey(0);

                    strStream.clear();

                    std::memset(buff, 0, sizeof(buff));

                    more_resize = consoleIOPtr->Input<bool>(TEXT("Would you like another resize operation? Y | N"), Str_To_Bool);

                } while (more_resize);

                
                bool need_roi = consoleIOPtr->Input<bool>(TEXT("Would you like to use ROI? Y | N"), Str_To_Bool);
                
                bool more_roi = false;

                if (need_roi)
                {
                    do
                    {
                        int x = consoleIOPtr->Input<int>(TEXT("Enter the x position for ROI:"), Str_to_int, int_positive_Validator_0);

                        int y = consoleIOPtr->Input<int>(TEXT("Enter the y position for ROI:"), Str_to_int, int_positive_Validator_0);

                        int width = consoleIOPtr->Input<int>(TEXT("Enter width for ROI:"), Str_to_int, int_positive_Validator_0);

                        int height = consoleIOPtr->Input<int>(TEXT("Enter height for ROI:"), Str_to_int, int_positive_Validator_0);

                        int scalar = consoleIOPtr->Input<int>(TEXT("Enter scalar for ROI:"), Str_to_int, int_positive_Validator_0);
                        
                        IplImage* last = Images[Images.size() - 1];

                        std::stringstream strStr;

                        strStr << "ROI" << "pos:[" << x << "," << y << "]" << "size: [" << width << "," << height << "]";

                        char buff[20];

                        strStr.getline(buff, sizeof(buff));

                        cvNamedWindow(buff, CV_WINDOW_AUTOSIZE);

                        cvSetImageROI(last, cvRect(x, y, width, height));

                        cvAddS(last, cvScalar(scalar), last);

                        cvShowImage(buff, last);

                        cv::waitKey(0);

                        more_roi = consoleIOPtr->Input<bool>(TEXT("Would you like to select another ROI? Y | N"), Str_To_Bool);

                        cvResetImageROI(last);

                        std::memset(buff, 0, sizeof(buff));

                    } while (more_roi);
                }
            }

            size_t len = Images.size();

            for (size_t i = 0; i < len; i++)
            {
                cvReleaseImage(&Images[i]);
            }

            cvReleaseImage(&srcImg);
        }   

        bool once_more = consoleIOPtr->Input<bool>(TEXT("Would you like to process another image? Y | N"), Str_To_Bool);

        if (!once_more)
            break;
    }

    printf("Finish!\n");

    cvDestroyAllWindows();

    return EXIT_SUCCESS;
}


