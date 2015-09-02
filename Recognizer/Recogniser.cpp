#pragma once
#include "recogniser.h"
Recogniser::Recogniser()
{
	results_path = 0;
	templates_path = 0;
}

//як параметр передаЇтьс€ шл€х дл€ збер≥ганн€ результат≥в та шаблон≥в дл€ пор≥вн€нн€
Recogniser::Recogniser(char* new_results_path, char* new_templates_path)
{
	results_path = new_results_path;
	templates_path = new_templates_path;
}

Recogniser::~Recogniser()
{
	cvDestroyAllWindows();
}

int Recogniser::findCircle(int argc, char* argv[])
{
	char file1[] = "C:/tst/15.jpg";

	int ch = '/';
	char *name;
	char result_name[100] = "result";
	name = strrchr(file1, ch);
	std::strcat(result_name, name);
	result_name[6] = '_';
	char result_path[250] = "";
	std::strncpy(result_path, results_path, 250);


	IplImage* image = 0;
	// им€ картинки задаЄтс€ первым параметром
	char* filename = argc >= 1 ? argv[0] : file1;
	// получаем картинку (в градаци€х серого)
	image = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);

	printf("[i] image: %s\n", filename);
	//assert(image != 0);
	if (image == 0)
		return -1;

	// загрузим оригинальное изображении
	IplImage* src = cvLoadImage(filename);
	IplImage* src_rio = cvLoadImage(filename);

	// хранилище пам€ти дл€ кругов
	CvMemStorage* storage = cvCreateMemStorage(0);
	// сглаживаем изображение
	cvSmooth(image, image, CV_GAUSSIAN, 3, 3);
	// поиск кругов
	CvSeq* results = cvHoughCircles(
		image,
		storage,
		CV_HOUGH_GRADIENT,
		1,
		image->width / 2
		);
	// пробегаемс€ по кругам и рисуем их на оригинальном изображении
	for (int i = 0; i < results->total; i++) {
		float* p = (float*)cvGetSeqElem(results, i);
		CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));
		cvCircle(src, pt, cvRound(p[2]), CV_RGB(0xff, 0, 0));
		int x = (p[0] - p[2] - p[2] * 0.25);
		int y = (p[1] - p[2] - p[2] * 0.25);

		cvSetImageROI(src_rio, cvRect(x, y, p[2] * 2.5, p[2] * 2.5));

	}
	// показываем
	//cvNamedWindow("cvHoughCircles", 1);
	//cvShowImage("cvHoughCircles", src);
	//cvShowImage("ROI", src_rio);

	//cvAddS(image, cvScalar(add), image);
	// сбрасываем ROI
	//cvResetImageROI(src);
	// показываем изображение

				

	// ждЄм нажати€ клавиши
	//cvWaitKey(0);

	//«бер≥гаю печатку
	char* signet_path = new char[250];
	std::strncpy(signet_path, result_path, 250);
	cvSaveImage(std::strncat(signet_path, "signet.jpg", 250), src_rio);

	cvSaveImage(std::strncat(result_path, result_name, 250), src);
	// освобождаем ресурсы
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&image);
	cvReleaseImage(&src);
	cvDestroyAllWindows();
	delete signet_path;
	return 0;
}

int Recogniser::matchTemplate(int argc, char* argv[]) {
	IplImage *src, *templ, *ftmp[6]; //ftmp is what to display on
	int i;
	char file1[] = "C:/tst/13.jpg";
	char file2[] = "C:/tst/11.jpg";
	char* filename = argc >= 2 ? argv[1] : file1;
	char* filename0 = argc >= 3 ? argv[2] : file2;


	// загрузка изображени€
	if ((src = cvLoadImage(filename, 1)) == 0) {
		printf("Error on reading src image %s\n", argv[1]);
		return(-1);
	}

	// загрузка шаблона
	if ((templ = cvLoadImage(filename0, 1)) == 0) {
		printf("Error on reading template %s\n", argv[2]);
		return(-1);
	}

	int patchx = templ->width;
	int patchy = templ->height;
	int iwidth = src->width - patchx + 1;
	int iheight = src->height - patchy + 1;
	for (i = 0; i<6; ++i){
		ftmp[i] = cvCreateImage(cvSize(iwidth, iheight), 32, 1);
	}

	// сравнение шаблона с изображением
	for (i = 0; i<6; ++i){
		cvMatchTemplate(src, templ, ftmp[i], i);
		//              double min,max;
		//              cvMinMaxLoc(ftmp,&min,&max);
		cvNormalize(ftmp[i], ftmp[i], 1, 0, CV_MINMAX);
	}
	// показываем
	cvNamedWindow("Template");
	cvShowImage("Template", templ);
	cvNamedWindow("Image");
	cvShowImage("Image", src);

	cvNamedWindow("SQDIFF");
	cvShowImage("SQDIFF", ftmp[0]);

	cvNamedWindow("SQDIFF_NORMED");
	cvShowImage("SQDIFF_NORMED", ftmp[1]);

	cvNamedWindow("CCORR");
	cvShowImage("CCORR", ftmp[2]);

	cvNamedWindow("CCORR_NORMED");
	cvShowImage("CCORR_NORMED", ftmp[3]);

	cvNamedWindow("CCOEFF");
	cvShowImage("CCOEFF", ftmp[4]);

	cvNamedWindow("CCOEFF_NORMED");
	cvShowImage("CCOEFF_NORMED", ftmp[5]);
	cvWaitKey(0);
}

//http://rosettacode.org/wiki/Percentage_difference_between_images
//–≥зниц€ м≥ж картинками у в≥дсотках, поп≥ксельно
double Recogniser::percentageDiff(char* file1, char* file2)
{
	IplImage *image1, *image2;
	if ((image1 = cvLoadImage(file1, 1)) == 0) {
		printf("Error on reading src image %s\n", file1);
		return(-1);
	}

	// загрузка шаблона
	if ((image2 = cvLoadImage(file2, 1)) == 0) {
		printf("Error on reading template %s\n", file2);
		return(-1);
	}

	int width1 = image1->width;
	int width2 = image2->width;
	int height1 = image1->height;
	int height2 = image2->height;

	if ((width1 != width2) || (height1 != height2)) {
		printf("Error: Images dimensions mismatch");
		return -2;
	}
	cv::Mat matrix1(image1);
	cv::Mat matrix2(image2);
	double totalDiff = 0.0;
	for (int y = 0; y < image1->height; y++)
		for (int x = 0; x < image1->width; x++)
		{
			totalDiff += abs(matrix1.at<cv::Vec3b>(y, x)[0] - matrix2.at<cv::Vec3b>(y, x)[0]) / 255.0;
			totalDiff += abs(matrix1.at<cv::Vec3b>(y, x)[1] - matrix2.at<cv::Vec3b>(y, x)[1]) / 255.0;
			totalDiff += abs(matrix1.at<cv::Vec3b>(y, x)[2] - matrix2.at<cv::Vec3b>(y, x)[2]) / 255.0;
		}
	
    auto result = 100.0 * totalDiff / (double)(image1->width * image2->height * 3);
	cvReleaseImage(&image1);
	cvReleaseImage(&image2);
	return result;
}

__int64 Recogniser::calcHammingDistance(__int64 x, __int64 y)
{
	__int64 dist = 0, val = x ^ y;

	// Count the number of set bits
	while (val)
	{
		++dist;
		val &= val - 1;
	}

	return dist;
}

// рассчитать хеш картинки
__int64 Recogniser::calcImageHash(IplImage* src, bool show_results = false)
{
	if (!src){
		return 0;
	}

	IplImage *res = 0, *gray = 0, *bin = 0;

	res = cvCreateImage(cvSize(8, 8), src->depth, src->nChannels);
	gray = cvCreateImage(cvSize(8, 8), IPL_DEPTH_8U, 1);
	bin = cvCreateImage(cvSize(8, 8), IPL_DEPTH_8U, 1);

	// уменьшаем картинку
	cvResize(src, res);
	// переводим в градации серого
	cvCvtColor(res, gray, CV_BGR2GRAY);
	// вычисл€ем среднее
	CvScalar average = cvAvg(gray);
	printf("[i] average: %.2f \n", average.val[0]);
	// получим бинарное изображение относительно среднего
	// дл€ этого воспользуемс€ пороговым преобразованием
	cvThreshold(gray, bin, average.val[0], 255, CV_THRESH_BINARY);

	// построим хэш
	__int64 hash = 0;

	int i = 0;
	// пробегаемс€ по всем пиксел€м изображени€
	for (int y = 0; y<bin->height; y++) {
		uchar* ptr = (uchar*)(bin->imageData + y * bin->widthStep);
		for (int x = 0; x<bin->width; x++) {
			// 1 канал
			if (ptr[x]){
				// hash |= 1<<i;  // warning C4334: '<<' : result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
				hash |= 1i64 << i;
			}
			i++;
		}
	}

	printf("[i] hash: %I64X \n", hash);

	//if (show_results){
		// увеличенные картинки дл€ отображени€ результатов
	//	IplImage* dst3 = cvCreateImage(cvSize(128, 128), IPL_DEPTH_8U, 3);
	//	IplImage* dst1 = cvCreateImage(cvSize(128, 128), IPL_DEPTH_8U, 1);

	//	// показываем картинки
	//	cvNamedWindow("64");
	//	cvResize(res, dst3, CV_INTER_NN);
	//	cvShowImage("64", dst3);
	//	cvNamedWindow("gray");
	//	cvResize(gray, dst1, CV_INTER_NN);
	//	cvShowImage("gray", dst1);
	//	cvNamedWindow("bin");
	//	cvResize(bin, dst1, CV_INTER_NN);
	//	cvShowImage("bin", dst1);

	//	cvReleaseImage(&dst3);
	//	cvReleaseImage(&dst1);
	//}

	// освобождаем ресурсы
	cvReleaseImage(&res);
	cvReleaseImage(&gray);
	cvReleaseImage(&bin);

	return hash;
}

__int64 Recogniser::pictureDiffHash(char* object_filename, char* image_filename){
	IplImage *object = 0, *image = 0;

	char obj_name[] = "C:/tst/10.jpg";
	char img_name[] = "C:/tst/10_1.jpg";

	// им€ объекта задаЄтс€ первым параметром
	//char* object_filename = argc >= 2 ? argv[1] : obj_name;
	// им€ картинки задаЄтс€ вторым параметром
	//char* image_filename = argc >= 3 ? argv[2] : img_name;

	// получаем картинку
	object = cvLoadImage(object_filename, 1);
	image = cvLoadImage(image_filename, 1);

	printf("[i] object: %s\n", object_filename);
	printf("[i] image: %s\n", image_filename);
	if (!object){
		printf("[!] Error: cant load object image: %s\n", object_filename);
		return -1;
	}
	if (!image){
		printf("[!] Error: cant load test image: %s\n", image_filename);
		return -1;
	}

	// покажем изображение
	// cvNamedWindow("object");
	//cvShowImage("object", object);
	//cvNamedWindow("image");
	//cvShowImage("image", image);

	// построим хэш
	__int64 hashO = calcImageHash(object, true);
	//cvWaitKey(0);
	__int64 hashI = calcImageHash(image, false);

	// рассчитаем рассто€ние ’эмминга
	__int64 dist = calcHammingDistance(hashO, hashI);

	//printf("[i] Hamming distance: %I64d \n", dist);

	// ждЄм нажати€ клавиши
	//cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&object);
	cvReleaseImage(&image);

	// удал€ем окна
	//cvDestroyAllWindows();
	return dist;
}