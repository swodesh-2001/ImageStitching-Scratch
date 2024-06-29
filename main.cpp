 #include "stitch.h"
 
int main(int argc ,char* argv[]){
    if(argc < 3){
        std::cerr << "Provide valid image path" << std::endl;
    }

    Mat img1 = imread(argv[1]);
    Mat img2 = imread(argv[2]);

    Mat combination;
    combination = combine_image(img1 ,img2);
    //imwrite("../images/blended_image.jpg", result_blended);
    return 0;
}


  