#include "warp.h"
#include "homography.h"
 
cv::Mat crop_pic(cv::Mat image, std::vector<int> roi) {
    // Check if the size of roi is at least 4
    cv::Rect myROI{roi[0], roi[1], roi[2], roi[3]};
    cv::Mat croppedImage = image(myROI);
    return croppedImage;
}
 


cv::Mat stitch_image(cv::Mat  const  &destination_image, cv::Mat  const &source_image, int tx, int ty) {
    int rows_base = destination_image.rows;
    int columns_base = destination_image.cols;
    int rows_image = source_image.rows;
    int columns_image = source_image.cols;

    int x_min = 0 , y_min = 0;
    int x_max = 0 , y_max = 0;
   
    if (tx > 0){
        x_max = std::max(tx + columns_image, columns_base);
        }
    else{
        x_max = std::max(abs(tx) + columns_base, columns_image);
        }

    if (ty > 0){
        y_max = std::max(ty + rows_image, rows_base);
    }
    else{
        y_max = std::max(std::abs(ty) + rows_base, rows_image);
    }
    
    int blending_window = 40 ;
    int size_x = std::ceil(x_max - x_min);
    int size_y = std::ceil(y_max - y_min);
    std::cout << "Size x : " << size_x << endl;
    std::cout << "Size y : " << size_y << endl;
    cv::Mat blending_1 = cv::Mat::zeros(size_y, size_x, CV_8UC3);
    cv::Mat blending_2 = cv::Mat::zeros(size_y, size_x, CV_8UC3);
    cv::Mat blended = cv::Mat::zeros(size_y, size_x, CV_8UC3);
    // converting both to same dimension

    if (tx > 0) {
        for(int i = 0 ; i < rows_image ; i++){
            for (int j = abs(columns_base - tx - 2 * blending_window), c = 0  ; j < columns_image; j++ , c++)
            {
                if(c < 2 * blending_window){  
                    blending_1.at<cv::Vec3b>(i + ty, j + tx)[0] = int(float(source_image.at<cv::Vec3b>(i,j)[0] ) *( float(c)/float(2 * blending_window))); 
                    blending_1.at<cv::Vec3b>(i + ty, j + tx)[1] = int(float(source_image.at<cv::Vec3b>(i,j)[1]) *( float(c)/float(2 * blending_window))); 
                    blending_1.at<cv::Vec3b>(i + ty, j + tx)[2] = int(float(source_image.at<cv::Vec3b>(i,j)[2]) *( float(c)/float(2 * blending_window)));  

                }
                else {
                    blending_1.at<cv::Vec3b>(i + ty, j + tx) = source_image.at<cv::Vec3b>(i,j);
                    
                } 
            }     
        }


        for(int i = 0; i < rows_base ; i++){
            for (int j = 0; j < columns_base; j++)
            {       
                if(j > (columns_base - 2 * blending_window) ){
                    blending_2.at<cv::Vec3b>(i, j)[0] = int(float(destination_image.at<cv::Vec3b>(i,j)[0]) * (1. - float(j+ 2 * blending_window-columns_base)/float(2 * blending_window))); // Blue channel
                    blending_2.at<cv::Vec3b>(i, j)[1] = int(float(destination_image.at<cv::Vec3b>(i,j)[1]) * (1. - float(j+ 2 * blending_window-columns_base)/float(2 * blending_window))); // Green channel
                    blending_2.at<cv::Vec3b>(i, j)[2] = int(float(destination_image.at<cv::Vec3b>(i,j)[2]) * (1. - float(j+ 2 * blending_window-columns_base)/float(2 * blending_window))); // Red channel
                }
                else{
                    blending_2.at<Vec3b>(i, j) = destination_image.at<Vec3b>(i, j);
                 }
                } 
        }
 
       }

    else {  
        
            for(int i = 0 ; i < rows_base ; i++){
            for (int j = 0  ; j < columns_base; j++)
            {
                if(j < 2 * blending_window){  
                    blending_1.at<cv::Vec3b>(i + abs(ty), j + abs(tx))[0] = int(float(destination_image.at<cv::Vec3b>(i,j)[0] ) *( float(j)/float(2 * blending_window))); 
                    blending_1.at<cv::Vec3b>(i + abs(ty), j + abs(tx))[1] = int(float(destination_image.at<cv::Vec3b>(i,j)[1]) *( float(j)/float(2 * blending_window))); 
                    blending_1.at<cv::Vec3b>(i + abs(ty), j + abs(tx))[2] = int(float(destination_image.at<cv::Vec3b>(i,j)[2]) *( float(j)/float(2 * blending_window)));  

                }
                else {
                    blending_1.at<cv::Vec3b>(i + abs(ty), j + abs(tx)) = destination_image.at<cv::Vec3b>(i,j);
                    
                } 
            }     
        }


        for(int i = 0; i < rows_image; i++){
            for (int j = 0; j < abs(tx) + 2 * blending_window; j++)
            {       
                if(j > abs(tx)){
                    blending_2.at<cv::Vec3b>(i, j)[0] = int(float(source_image.at<cv::Vec3b>(i,j)[0]) * (1. - float(j - abs(tx))/float(2 * blending_window))); // Blue channel
                    blending_2.at<cv::Vec3b>(i, j)[1] = int(float(source_image.at<cv::Vec3b>(i,j)[1]) * (1. - float(j - abs(tx))/float(2 * blending_window))); // Green channel
                    blending_2.at<cv::Vec3b>(i, j)[2] = int(float(source_image.at<cv::Vec3b>(i,j)[2]) * (1. - float(j - abs(tx))/float(2 * blending_window))); // Red channel
                }
                else{
                    blending_2.at<Vec3b>(i, j) = source_image.at<Vec3b>(i, j);
                 }
                } 
        }

         }

    cv::addWeighted(blending_1,1,blending_2,1,0,blended);
    namedWindow("Result 1",WINDOW_NORMAL);
    resizeWindow("Result 1" ,{500,500});
    imshow("Result 1", blending_1) ;

    namedWindow("Result 2",WINDOW_NORMAL);
    resizeWindow("Result 2" ,{500,500});
    imshow("Result 2", blending_2);
    waitKey(0);



    std::cout << "Blending is completed." << std::endl;
    return crop_pic(blended,{0,abs(ty),size_x,rows_base - abs(ty)});
}




cv::Mat combine_image(cv::Mat img1,cv::Mat img2){

    Mat gray1,gray2;
    cvtColor(img1, gray1, COLOR_BGR2GRAY);
    cvtColor(img2, gray2, COLOR_BGR2GRAY);

    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    
    auto detector  = cv::xfeatures2d::SurfFeatureDetector::create();
    detector->detectAndCompute(gray1,noArray(),keypoints_1,descriptors_1); 
    detector->detectAndCompute(gray2, noArray(), keypoints_2, descriptors_2);
 
    Mat img1_keypoints,img2_keypoints;
    drawKeypoints( img1, keypoints_1, img1_keypoints);
    drawKeypoints( img2, keypoints_2, img2_keypoints);
    namedWindow("SURF Keypoints 1",WINDOW_NORMAL);
    resizeWindow("SURF Keypoints 1",{500,500});
    namedWindow("SURF Keypoints 2",WINDOW_NORMAL);
    resizeWindow("SURF Keypoints 2",{500,500});
    imshow("SURF Keypoints 1", img1_keypoints );
    imshow("SURF Keypoints 2", img1_keypoints );
    cv:waitKey(0);


    BFMatcher bf(NORM_L2, true);
    vector<DMatch> matches,matches_display;
    bf.match(descriptors_1, descriptors_2, matches);
    sort(matches.begin(), matches.end(), [](const DMatch& a, const DMatch& b) {
        return a.distance < b.distance;
    });

    matches_display = std::vector<DMatch>(matches.begin() , matches.begin() + 50);

    Mat img3;
    drawMatches(gray1, keypoints_1, gray2, keypoints_2, matches_display, img3, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    resize(img3, img3, Size(1000, 400));
    imshow("SURF Matches", img3);
    waitKey(0);

    vector<Point2f> img1_points, img2_points;
    for (auto match : matches) {
        int query_index = match.queryIdx;
        int train_index = match.trainIdx;
        float x1 = keypoints_1[query_index].pt.x, y1 = keypoints_1[query_index].pt.y;
        float x2 = keypoints_2[train_index].pt.x, y2 = keypoints_2[train_index].pt.y;
        img1_points.push_back(Point2f(x1, y1));
        img2_points.push_back(Point2f(x2, y2));
    }
    vector<Point2f> selected_img1_points(img1_points.begin(), img1_points.begin() + 100);
    vector<Point2f> selected_img2_points(img2_points.begin(), img2_points.begin() + 100);
    
    Mat homography_matrix = find_homography_matrix(selected_img1_points,selected_img2_points);
    auto result = image_warp(img1 , homography_matrix);
    cv::Mat warped_image = std::get<0>(result);
    int tx = std::get<1>(result);
    int ty = std::get<2>(result);
    Mat result_blended = stitch_image(img2, warped_image,tx, ty);
    namedWindow("Result",WINDOW_NORMAL);
    resizeWindow("Result" ,{500,500});
    imshow("Result", result_blended);
    waitKey(0);
    return result_blended;

}

