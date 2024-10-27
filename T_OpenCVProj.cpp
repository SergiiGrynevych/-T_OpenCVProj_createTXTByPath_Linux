#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <filesystem>
#include <fstream>
#include <optional>

namespace fs = std::filesystem;
using namespace std;
using namespace cv;

struct ImageDimension
{
    int width;
    int height;
};

ImageDimension SaveImageFileNames(const string &imagePath, bool &isImageFilenames)
{
    ImageDimension dimensions = {0, 0};

    Mat image = imread(imagePath);
    if (image.empty())
    {
        cerr << "Failed to load image: " << imagePath << endl;
        isImageFilenames = false;
        return dimensions;
    }

    dimensions.width = image.cols;
    dimensions.height = image.rows;

    isImageFilenames = true;
    return dimensions;
}

bool WriteImageDataFile(const string &outputFile, ImageDimension &img, const string &fileName)
{
    try
    {
        ofstream outFile(outputFile, ios_base::app);
        if (!outFile)
        {
            std::cerr << "Error when create outFile." << std::endl;
            return false;
        }

        outFile << "File: " << fileName << ", width: " << img.width << ", height: " << img.height << std::endl;

        if (!outFile)
        {
            cerr << "Error when write outFile." << std::endl;
            return false;
        }

        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        cerr << "Filesystem error: " << e.what() << endl;
        cerr << "Error code: " << e.code().value() << " - " << e.code().message() << endl;
        return false;
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        return false;
    }
}

void SaveWriteDataFile(const string &directoryPath, bool &isImageFilenames, const string &outputFile, const string &fileName)
{
    ImageDimension dim = SaveImageFileNames(directoryPath, isImageFilenames);

    if (isImageFilenames)
    {
        isImageFilenames = false;
        bool isWrite = WriteImageDataFile(outputFile, dim, fileName);
        if (!isWrite)
        {
            cerr << "Error when write file: " << fileName << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Need to enter 2 args. 1. DirectoryPath; 2. OutputFile" << std::endl;
        return 1;
    }

    string fileName = "";
    bool isImageFilenames = false;
    string defaultNameFile = "defaultNameFile.txt";

    string directoryPath = argv[1]; // "/home/serg/Documents/programming/T_OpenCVProj_createTXTByPath-master/IMG_DIR/cat_test_photo.jpg"; //
    string outputFile = argv[2];    // "/home/serg/Documents/programming/test1111111111.txt";

    if (outputFile.substr(outputFile.length() - 4) != ".txt")
    {
        if (outputFile.substr(outputFile.length() - 1) != "/")
        {
            defaultNameFile = "/" + defaultNameFile;
        }
        outputFile += defaultNameFile;
    }

    if (!fs::exists(directoryPath) /*|| !fs::is_directory(directoryPath)*/)
    {
        cerr << "Directory does not exist or is not a directory." << " DiectoryPath: " << directoryPath << endl;
        return false;
    }

    if (fs::is_directory(directoryPath))
    {
        for (const auto &entry : fs::directory_iterator(directoryPath))
        {
            if (entry.is_regular_file())
            {
                fileName = entry.path().filename().string();
                string fileExtension = entry.path().extension().string();
                if (fileExtension == ".jpg" || fileExtension == ".png" || fileExtension == ".jpeg")
                {
                    SaveWriteDataFile(entry.path().string(), isImageFilenames, outputFile, fileName);
                }
            }
        }
    }
    else
    {
        // string fileExtension = "";
        size_t slashPosition = directoryPath.find_last_of("/\\");
        // Check if a slash was found
        if (slashPosition != string::npos)
        {
            fileName = directoryPath.substr(slashPosition + 1); // Return substring after the last slash

            /*
            size_t dotPosition = fileName.rfind('.');
            // Check if a dot was found and it's not the first character
            if (dotPosition != std::string::npos && dotPosition != 0)
            {
                fileExtension = fileName.substr(dotPosition); // Return the substring from the dot to the end
            }
            */
        }

        SaveWriteDataFile(directoryPath, isImageFilenames, outputFile, fileName);
    }
    cout << "-----------" << endl
         << "End program. Thanks!" << endl;

    return 0;
}
