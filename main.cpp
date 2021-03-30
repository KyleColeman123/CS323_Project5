#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

class Image{
	public:
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	
	int power2Size;
	int** imgAry;
	
	Image(int r, int c, int minV, int maxV){
		int pwr2 = computePower2(r, c);
		numRows = r;
		numCols = c;
		minVal = minV;
		maxVal = maxV;
		imgAry = new int*[pwr2];
		for(int i = 0; i < pwr2; i++){
    		imgAry[i] = new int[pwr2];
    	}
	}
	
	int computePower2(int nR, int nC){
		int size = max(nR, nC);
		int power2 = 2;
		while (size>power2){
			if (size>power2) power2 *= 2;
		}
		return power2;
	}
	void zero2DAry (){
		for (int i=0; i<power2Size; i++){
			for (int j=0; j<power2Size; j++){
				imgAry[i][j] = 0;	
			}
		}
	}
	void loadImage (ifstream &inFile, ofstream &outFile){
		for (int i=0; i<power2Size; i++){
			for (int j=0; j<power2Size; j++){
				if (j<numCols)
					inFile >> imgAry[i][j];					
					outFile<<imgAry[i][j]<<" ";	
			}
			outFile<<endl;
		}
	}
		
};

class QtTreeNode{
	public:
	int color;
	int upperR=0;
	int upperC=0;
	int Size;
	QtTreeNode* NWkid = NULL;
	QtTreeNode* NEkid = NULL;
	QtTreeNode* SWkid = NULL;
	QtTreeNode* SEkid = NULL;
	
	QtTreeNode(int uR, int uC, int s, QtTreeNode* nw, QtTreeNode* ne, QtTreeNode* sw, QtTreeNode* se){
		upperR = uR;
		upperC = uC;
		Size = s;
		NWkid = nw;
		NEkid = ne;
		SWkid = sw;
		SEkid = se;	
	}
	bool isLeaf (){
		if (this->NWkid == NULL && this->NEkid == NULL 
		&& this->SWkid == NULL && this->SEkid == NULL) return true;
		return false;
	}
	void printQTNode(ofstream &outFile){
		outFile<<"Color:"<<this->color<<" "
		<<"UpperRow:"<<this->upperR<<" "
		<<"UpperCol:"<<this->upperC<<" ";
		if (this->NWkid != NULL) outFile<<"NWColor:"<<this->NWkid->color<<" ";
		else outFile<<"NWColor:"<<"null ";
		if (this->NEkid != NULL) outFile<<"NEColor:"<<this->NEkid->color<<" ";
		else outFile<<"NEColor:"<<"null ";
		if (this->SWkid != NULL) outFile<<"SWColor:"<<this->SWkid->color<<" ";
		else outFile<<"SWColor:"<<"null ";
		if (this->SEkid != NULL) outFile<<"SEColor:"<<this->SEkid->color<<" ";
		else outFile<<"SEColor:"<<"null ";
		outFile<<endl;
	}
};

class QuadTree{
	public:
	QtTreeNode* QtRoot;
	
	QuadTree(){	
	}
	QtTreeNode* buildQuadTree(int** imgAry, int upR, int upC, int size, ofstream& outFile){
		QtTreeNode* newQtNode = new QtTreeNode(upR, upC, size, NULL, NULL, NULL, NULL);
		newQtNode->printQTNode(outFile);
		if (size == 1) newQtNode->color = imgAry[upR][upC];
		else{
			int halfSize = size/2;
			newQtNode->NWkid = buildQuadTree(imgAry, upR, upC, halfSize, outFile);
			newQtNode->NEkid = buildQuadTree(imgAry, upR, upC + halfSize, halfSize, outFile);
			newQtNode->SWkid = buildQuadTree(imgAry, upR + halfSize, upC, halfSize, outFile);
			newQtNode->SEkid = buildQuadTree(imgAry, upR + halfSize, upC + halfSize, halfSize, outFile);
			int sumColor = newQtNode->NWkid->color + newQtNode->NEkid->color 
			+ newQtNode->SWkid->color + newQtNode->SEkid->color;
			if  (sumColor == 0){
				newQtNode->color = 0;
				newQtNode->NWkid = NULL;
				newQtNode->NEkid = NULL;
				newQtNode->SWkid = NULL;
				newQtNode->SEkid = NULL;
			}
			else if  (sumColor == 4){
				newQtNode->color = 1;
				newQtNode->NWkid = NULL;
				newQtNode->NEkid = NULL;
				newQtNode->SWkid = NULL;
				newQtNode->SEkid = NULL;
			}
			else newQtNode->color = 5;	
		}
		return newQtNode;
	}
	void preOrder(QtTreeNode *Qt, ofstream& outFile){
		if (Qt->isLeaf()) Qt->printQTNode(outFile);
		else{
			Qt->printQTNode(outFile);
			preOrder (Qt->NWkid, outFile);
			preOrder (Qt->NEkid, outFile);
			preOrder (Qt->SWkid, outFile);
			preOrder (Qt->SEkid, outFile);
		}
	}
	void postOrder(QtTreeNode *Qt, ofstream& outFile){
		if (Qt->isLeaf()) Qt->printQTNode(outFile);
		else{
			postOrder (Qt->NWkid, outFile);
			postOrder (Qt->NEkid, outFile);
			postOrder (Qt->SWkid, outFile);
			postOrder (Qt->SEkid, outFile);
			Qt->printQTNode(outFile);
		}
	}
};

int main (int argc, char *argv[]){
	ifstream input;
	input.open(argv[1]);
	
	ofstream output1, output2;
	output1.open(argv[2]);
	output2.open(argv[3]);
	int numRows, numCols, minVal, maxVal;
	while (!input.eof()){
		input >> numRows;
		input >> numCols;
		input >> minVal;
		input >> maxVal;
		break;
	}
	Image myImage(numRows, numCols, minVal, maxVal);
	myImage.power2Size = myImage.computePower2(numRows, numCols);
	output2<<"The power of 2 is: "<<myImage.power2Size<<endl;
	myImage.zero2DAry();
	output2<<"\nThe Image\n";
	myImage.loadImage(input, output2);
	QuadTree qTree;
	qTree.QtRoot = qTree.buildQuadTree(myImage.imgAry, 0, 0, myImage.power2Size, output2);
	output1<<"Preorder Traversal:\n";
	qTree.preOrder(qTree.QtRoot, output1);
	output1<<endl<<"Postorder Traversal:\n";
	qTree.postOrder(qTree.QtRoot, output1);
	cout<<"\nDone";
	return 0;
}
