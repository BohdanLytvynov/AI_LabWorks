
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include <windows.h>
#include "../ConsoleUI/console_ui.h"
#include <tchar.h>
#include"../ConsoleColors/console_colors.h"
#include<vector>
#include<sstream>
#include<stack>
#include"Processed_Image.h"


/// <summary>
/// Currently not used but maybe we will use it in future
/// </summary>
/// <returns></returns>
LPCTSTR GetCurrentFile()
{
#ifdef _UNICODE
    return __FILEW__;
#else
    return __FILE__;
#endif // _UNICODE  
}

const int count_to_ignore = 8;
/// <summary>
/// Currently not used
/// </summary>
/// <param name="size_of_input"></param>
/// <param name="count_to_ignore"></param>
/// <param name="result"></param>
void getPathToCurrentFolder(int size_of_input, int count_to_ignore, LPTSTR result)
{
    LPCTSTR input = GetCurrentFile();

    for (int i = 0; i < (size_of_input - count_to_ignore); i++)
    {
        result[i] = input[i];
    }
}
/// <summary>
/// Array of THAR that stores different variations of YES 
/// </summary>
LPCTSTR yes = TEXT("YyÕÌ");

/// <summary>
/// Array of THAR that stores different variations of NO 
/// </summary>
LPCTSTR no = TEXT("Nn“Ú");

/// <summary>
/// TCHAR string to bool converter function
/// </summary>
auto Str_To_Bool = [](LPCTSTR result, LPCTSTR& error, int& error_code)->bool
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

/// <summary>
/// TCHAR string to int converter function
/// </summary>
auto Str_to_int = [](LPCTSTR result, LPCTSTR& error, int& error_code) -> int
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

/// <summary>
/// Function that checks if double value is positive and not zero
/// </summary>
auto positive_double_validator = [](double result, LPCTSTR& error)->bool
    {
        if (result <= 0)
        {
            error = TEXT("Value can't ne zero or negative!");
            return false;
        }

        return true;
    };
/// <summary>
/// TCHAR string to double converter
/// </summary>
auto Str_to_double = [](LPCTSTR result, LPCTSTR& error, int& error_code) -> double
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

/// <summary>
/// Checks if the integer value is positive or 0
/// </summary>
auto int_positive_Validator_0 = [](int result, LPCTSTR& error) -> bool
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

void DestrImage(IplImage* img)
{
    cvReleaseImage(&img);
}

IplImage* Copy(IplImage* source)
{
    return cvCloneImage(source);
}

typedef Processed_Image<IplImage*> img;

//Temp storage for Images
std::stack<img> m_images;
//Temp storage for Window names
std::vector<const char*> m_windowNames;
//Last image that was processed in the current block
img m_lastImage;

void ClearStack(std::stack<img>* stack)
{
    while (!stack->empty())
        stack->pop();
}

void FinishProcessing()
{
    m_lastImage = img(m_images.top());

    ClearStack(&m_images);

    for (auto& window : m_windowNames)
    {
        cvDestroyWindow(window);
    }

    m_windowNames.clear();
}

int main()
{
    //Mem leak check flags setup
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);  

    printf("Lab work 1\n");

    printf("Program starting...\n");

    HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);//Get the pointer to the console
    //Init ConsoleInputOutput module that is required for IO operations
    //We used smart pointer and it will help us to manage heap
    std::shared_ptr<io::ConsoleInputOutput> consoleIOPtr = std::make_shared<io::ConsoleInputOutput>(handler);    
            
    //Main Programm Cycle
    //Controls if image was opened succesfully or not
    bool img_opened = false;

    IplImage* srcImg;//Pointer to the image

    CvRect m_roi;

    bool m_ROI_Used = false;

    //Main app cycle
    for (; ;)
    {
        //Get the path to the file that is the image for processing
        char* path = nullptr;
#if define(NDEBUG)//Release configuration
        consoleIOPtr->Input("Enter path to file:", path);
#else
        path = const_cast<char*>("E:\\AI\\CV\\C++\\AI_LabWorks\\Lab1\\Images\\plane.jpg");
#endif
        
        //Loading the image
        srcImg = cvLoadImage(path, CV_LOAD_IMAGE_UNCHANGED);
                
        if (srcImg)//Image successfuly loaded -> we can start the processing chain 
        {
            //Create the window to place the image there
            cvNamedWindow(originNameWindow, CV_WINDOW_AUTOSIZE);
            //Display the image
            cvShowImage(originNameWindow, srcImg);
            //Stop current thread to give some time for img loading and display
            cv::waitKey(0);
            img img("Original", srcImg, DestrImage, Copy);
            m_images.push(img);

            img_opened = true;
            FinishProcessing();
        }
        else//Case when the image wasn't loaded
        {
            consoleIOPtr->PrintLine("Unable to open file!");
            img_opened = false;
        }     

        if (img_opened)//Start the processing chain
        {
            //Do we need smoothing?
            bool need_smooth = consoleIOPtr->Input<bool>(TEXT("Would you like to smooth your image? Y | N"), Str_To_Bool);
            //Controls the current loop for smoothing
            bool tryMoreSmooth = false;

            if (need_smooth)//We need to smooth the image
            {        
                int index = 0;

                do
                {
                    //Clone the image for smooth, so we can compare it with the original one                 
                    IplImage* destImg = cvCloneImage(m_lastImage.getImage());
                    //Save new image to the Image storage so we will use the last processed image for later steps
                    img img("Smoothing", destImg, DestrImage, Copy);

                    m_images.push(img);

                    const char* windowName = "";

                    int smoothType = 0;
                    //Get the smoothing type
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
                    //Build new Window name with index
                    size_t count = std::strlen(windowName) + 10;
                    char* IndexedWindow = new char[count];

                    std::stringstream Str;

                    Str << windowName << " " << index;

                    Str.getline(IndexedWindow, count);

                    //Get the values that are required for smooth function
                    int size1 = consoleIOPtr->Input<int>(TEXT("Enter the value of size1 parameter."), Str_to_int);

                    int size2 = consoleIOPtr->Input<int>(TEXT("Enter the value of size2 parameter."), Str_to_int);

                    int sigma1 = consoleIOPtr->Input<int>(TEXT("Enter the value of sigma1 parameter."), Str_to_double);

                    int sigma2 = consoleIOPtr->Input<int>(TEXT("Enter the value of sigma2 parameter."), Str_to_double);
                    //Perform smoothing
                    cvSmooth(srcImg, destImg, smoothType, size1, size2, sigma1, sigma2);
                    //Create new window to show the result of smooth
                    cvNamedWindow(IndexedWindow, CV_WINDOW_AUTOSIZE);
                    //Show the smoothed image
                    cvShowImage(IndexedWindow, destImg);

                    //Add the window name to temp storage for this process block
                    m_windowNames.push_back(IndexedWindow);

                    Str.clear();//Clear StringStream

                    delete[] IndexedWindow;//Clear window name
                    IndexedWindow = nullptr;

                    //Current thread must wait until the smoothed image will be displayed
                    cv::waitKey(0);
                    //Do we need another iteration of smoothing?
                    tryMoreSmooth = consoleIOPtr->Input<bool>(TEXT("Would you like to try another smooth method? Y | N"), Str_To_Bool);                    

                    ++index;

                } while (tryMoreSmooth);

                FinishProcessing();
            }

            bool need_resize = consoleIOPtr->Input<bool>(TEXT("Would you like to resize your image? Y | N"), Str_To_Bool);
           
            if (need_resize)
            {                
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
                    
                    std::stringstream strStream;

                    strStream << "Resize" << "[" << w_multipl << "," << h_multipl << "]";

                    char buff[30];
                    strStream.getline(buff, sizeof(buff));

                    cvNamedWindow(buff, CV_WINDOW_AUTOSIZE);
                   
                    IplImage* resizedImg = cvCreateImage(cvSize(m_lastImage.getImage()->width / w_multipl, 
                        m_lastImage.getImage()->height / h_multipl), 
                        m_lastImage.getImage()->depth, m_lastImage.getImage()->nChannels);

                    cvResize(m_lastImage.getImage(), resizedImg, interpolation);

                    img img("Resize", resizedImg, DestrImage, Copy);

                    m_images.push(img);

                    cvShowImage(buff, resizedImg);

                    m_windowNames.push_back(buff);

                    cv::waitKey(0);

                    strStream.clear();

                    std::memset(buff, 0, sizeof(buff));

                    more_resize = consoleIOPtr->Input<bool>(TEXT("Would you like another resize operation? Y | N"), Str_To_Bool);

                } while (more_resize);

                FinishProcessing();                
            }
           
            bool need_roi = consoleIOPtr->Input<bool>(TEXT("Would you like to use ROI? Y | N"), Str_To_Bool);

            m_ROI_Used = need_roi;

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

                    IplImage* img = cvCloneImage(m_lastImage.getImage());

                    std::stringstream strStr;

                    strStr << "ROI" << "pos:[" << x << "," << y << "]" << "size: [" << width << "," << height << "]";

                    char buff[30];

                    strStr.getline(buff, sizeof(buff));

                    cvNamedWindow(buff, CV_WINDOW_AUTOSIZE);

                    m_roi = cvRect(x, y, width, height);

                    cvSetImageROI(img, m_roi);

                    cvAddS(img, cvScalar(scalar), img);
                    
                    cvResetImageROI(img);

                    cvShowImage(buff, img);

                    m_windowNames.push_back(buff);

                    cv::waitKey(0);

                    more_roi = consoleIOPtr->Input<bool>(TEXT("Would you like to select another ROI? Y | N"), Str_To_Bool);
                    
                    cvReleaseImage(&img);

                    cvDestroyWindow(buff);

                    std::memset(buff, 0, sizeof(buff));

                } while (more_roi);

                FinishProcessing();
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


