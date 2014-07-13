#include "kyheader.h"
#include "Objectness.h"
#include "ValStructVec.h"
#include "CmShow.h"
#include <algorithm>
#include <cmath>

int runBING(Mat& img, float boxes[], float conv5_windows[], const int boxes_num,
    const int max_size, const int min_size,
    const int conv5_hend, const int conv5_wend) {
	int base = 2, W = 8, NSS = 2, numPerSz = 250;
	DataSetVOC dataset;
	Objectness objNess(dataset, base, W, NSS);
	ValStructVec<float, Vec4i> boxesTests;
	objNess.getObjBndBoxesSingleImg(img, boxesTests, numPerSz);
	memset(boxes, 0, max_size*sizeof(float));
	int count = 0;
	for (int i = 0; i < boxesTests.size() && count < boxes_num; i++) {
	  int x1 = boxesTests[i][0];
	  int y1 = boxesTests[i][1];
	  int x2 = boxesTests[i][2];
	  int y2 = boxesTests[i][2];
	  bool valid = (x2 - x1 >= min_size) && (x2 - x1 <= max_size)
	      && (y2 - y1 >= min_size) && (y2 - y1 <= max_size);
	  if (valid) {
	    boxes[4*count  ] = float(y1 - 1);
	    boxes[4*count+1] = float(x1 - 1);
	    boxes[4*count+2] = float(y2 - 1);
	    boxes[4*count+3] = float(x2 - 1);
      conv5_windows[4*count  ] = float(max(cvRound((y1-17)/16), 0.f));
      conv5_windows[4*count+1] = float(max(cvRound((x1-17)/16), 0.f));
      conv5_windows[4*count+2] = float(min(cvRound((y2-17)/16)+1, float(conv5_hend)));
      conv5_windows[4*count+3] = float(min(cvRound((x2-17)/16)+1, float(conv5_wend)));
	    count++;
	  }
	}
	// append [0, 0, 0, 0] as ending mark
	if (count < boxes_num) {
	  boxes[4*count  ] = 0;
    boxes[4*count+1] = 0;
    boxes[4*count+2] = 0;
    boxes[4*count+3] = 0;
    conv5_windows[4*count  ] = 0;
    conv5_windows[4*count+1] = 0;
    conv5_windows[4*count+2] = 0;
    conv5_windows[4*count+3] = 0;
	}
	return count;
}

void runVOC(double base, int W, int NSS, int numPerSz)
{
	srand((unsigned int)time(NULL));
	DataSetVOC voc2007("/home/ronghang/workspace/VOC2007/");
	voc2007.loadAnnotations();

	printf("Dataset:`%s' with %d training and %d testing\n", _S(voc2007.wkDir), voc2007.trainNum, voc2007.testNum);
	printf("Base = %g, W = %d, NSS = %d, perSz = %d\n", base, W, NSS, numPerSz);

	Objectness objNess(voc2007, base, W, NSS);
	vector<vector<Vec4i> > boxesTests;
	objNess.getObjBndBoxesForTests(boxesTests, numPerSz);
}
