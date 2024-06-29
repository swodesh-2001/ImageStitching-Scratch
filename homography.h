#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>
#include <cmath> 
using namespace std;
using namespace cv;


void displayMatrix(const cv::Mat& matrix) {
    // Check if the matrix is empty
    if (matrix.empty()) {
        std::cout << "Matrix is empty!" << std::endl;
        return;
    }

    // Iterate over the matrix rows
    for (int i = 0; i < matrix.rows; ++i) {
        // Iterate over the matrix columns
        for (int j = 0; j < matrix.cols; ++j) {
            // Print the element at position (i, j)
            std::cout << matrix.at<float>(i, j) << "\t";
        }
        std::cout << std::endl; // Move to the next row
    }
}


Mat find_homography_matrix(vector<Point2f>& source_points, vector<Point2f>& destination_points) {
    // Check if the number of source points matches the number of destination points
    if (source_points.size() != destination_points.size() || source_points.size() < 4) {
        cerr << "Invalid number of points or insufficient points for homography estimation." << endl;
        return Mat();
    }

    // Find the homography matrix
    Mat homography = findHomography(source_points, destination_points, RANSAC);
    displayMatrix(homography);
    cout << "homography_calculated   \n " << endl;
    return homography;
}


cv::Mat vectorToMat(const std::vector<float>& data, int rows, int cols) {
    cv::Mat mat(rows, cols, CV_32FC1); // Initialize a CV_32FC1 (float) matrix

    // Copy data into the matrix
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mat.at<float>(i, j) = data[i * cols + j];
        }
    }
    return mat;
}

 


// cv::Mat find_homography_matrix(vector<Point2f> source_points, vector<Point2f> destination_points) {

//     int known_col = 2 * source_points.size();
//     std::vector<float> data;
//     for (int i = 0; i < source_points.size(); i++) {
//         float xs =  source_points[i].x, ys = source_points[i].y;
//         float xd = destination_points[i].x, yd = destination_points[i].y;
        
//         // Create the row vectors
//         float first_row_val[9] = {xs, ys, 1.0, 0, 0, 0, -xs * xd, -ys * xd, -xd};
//         float second_row_val[9] = {0, 0, 0, xs, ys, 1, -xs * yd, -ys * yd, -yd};
//         int first_array_size = sizeof(first_row_val) / sizeof(first_row_val[0]);
//         int second_array_size = sizeof(second_row_val) / sizeof(second_row_val[0]);
//         // Concatenate array_of_floats to vec
//         data.insert(data.end(), first_row_val, first_row_val+ first_array_size);
//         data.insert(data.end(), second_row_val, second_row_val+ second_array_size);

// }

    
//     cv::Mat A = vectorToMat(data, known_col, 9);
//     //cout << A.size() << endl;
//     //displayMatrix(A);
//     Mat A_transpose = A.t();
//     Mat A_mult = A * A_transpose;
//     cout << "....................... \n" << endl;
//     cout << A_mult.size() << endl;
//     cout << "....................... \n" << endl;


//     displayMatrix(A_mult);

//       cout << "....................... \n" << endl;
//         cout << "....................... \n" << endl;


//     Mat eigenvalues, eigenvectors;
//     cv::eigen(A_mult, eigenvalues, eigenvectors);
    
    
//     // Find the index of the smallest eigenvalue
//     int index_of_smallest_eigenvalue = 0;
//     for (int i = 1; i < eigenvalues.rows; ++i) {
//         if (eigenvalues.at<float>(i) < eigenvalues.at<float>(index_of_smallest_eigenvalue))
//             index_of_smallest_eigenvalue = i;
//     }
    
//     Mat smallest_eigenvector = eigenvectors.col(index_of_smallest_eigenvalue);
    
//     if (!smallest_eigenvector.isContinuous()) {
//         smallest_eigenvector = smallest_eigenvector.clone();
//     }

//     Mat homography_matrix = smallest_eigenvector.reshape(1, 3);
//     displayMatrix(homography_matrix);
//     return homography_matrix;
// }

