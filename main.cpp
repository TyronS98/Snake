#include <Windows.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <thread>
#include <chrono>

struct SSnake {
	unsigned short int static const iWidth = 20;
	unsigned short int static const iHeight = 20;
	
	unsigned short int xPosSnake = iWidth / 2;
	unsigned short int yPosSnake = iHeight / 2;
	unsigned short int xFruit = rand() % (iWidth-1) + 1;
	unsigned short int yFruit = rand() % (iHeight - 1) + 1;
	unsigned short int iMoveDirection = 0;

	unsigned short int xTail[iWidth];
	unsigned short int yTail[iHeight];

	unsigned short int maxTailLength = ((iWidth + iHeight) / 2);
	unsigned short int tailLength = 1; //((iWidth + iHeight) / 2); //(iWidth + iHeight)

	unsigned int iScore = 0;
	bool isDead = false;
	bool isDrawn;

	bool stopDraw = false;
};

void Draw(SSnake*);
void Logic(SSnake*);
void Input(SSnake*);
void SnakeReset(SSnake*);
void RunSnake();

/*
TEST CODE!!!!
*/

void externDrawing(SSnake*);

void CheckSnakeIndexThread(SSnake*);
void SnakeIndexCreaterThread(SSnake*);

void InitSnakeArray(SSnake*);
void ClearWindow();
void Test(SSnake*);

int main() {
	srand(time(NULL));

	while (!GetAsyncKeyState(VK_NUMPAD9) & 1) {
		RunSnake();


		//SSnake* s_Snake = RunSnake();
		//SnakeReset(s_Snake);
		//delete s_Snake;
	}

	return 0;
}

void RunSnake() {
	SSnake* s_Snake = new SSnake;

	while (!s_Snake->isDead) {

		s_Snake->xTail[0] = s_Snake->xPosSnake;
		s_Snake->yTail[0] = s_Snake->yPosSnake;

		//std::thread DrawThread(Draw, s_Snake);
		std::thread InputThread(Input, s_Snake);
		//std::thread LogicThread(Logic, s_Snake);
		std::thread CheckSnakeIndex(CheckSnakeIndexThread, s_Snake);

		Draw(s_Snake);
		//Input(s_Snake);
		Logic(s_Snake);

		//DrawThread.join();
		InputThread.join();
		//LogicThread.join();*/
		CheckSnakeIndex.join();

		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

	ClearWindow();

	delete s_Snake;

	//return s_Snake;
}

void Draw(SSnake* s_Snake) {
	ClearWindow();


	for (int i = 0; i < s_Snake->iWidth + 2; i++) {
		std::cout << "#";
	}

	for (int i = 0; i < s_Snake->iHeight; i++) {
		std::cout << "\n#";
		//std::cout << "#";

		for (int j = 0; j < s_Snake->iWidth; j++) {

			if (s_Snake->xPosSnake == j && s_Snake->yPosSnake == i) {
				std::cout << "O";
			}
			else if (s_Snake->xFruit == j && s_Snake->yFruit == i) {
				std::cout << "F";
			}
			else {
				s_Snake->isDrawn = false;
				for (int k = 0; k < s_Snake->maxTailLength; k++) {

					if (s_Snake->xTail[k] == j && s_Snake->yTail[k] == i) {
						std::cout << "o";
						s_Snake->isDrawn = true;
					}
				}
				if (!s_Snake->isDrawn) {
					std::cout << " ";
				}
			}
		}
		std::cout << "#";
	}


	/*for (int i = 0; i < s_Snake->iWidth; i++) {
		std::cout << "\n#";

		for (int j = 0; j < s_Snake->iHeight; j++) {
			std::cout << " ";
		}

		std::cout << "#";
	}*/

	std::cout << "\n";
	for (int i = 0; i < s_Snake->iWidth + 2; i++) {
		std::cout << "#";
	}


	std::cout << "\nSCORE: " << s_Snake->iScore;
}

void Logic(SSnake* s_Snake) {
	int xPosPrev = s_Snake->xTail[0];
	int yPosPrev = s_Snake->yTail[0];
	//std::cout << "("<< xPosPrev << "," << yPosPrev << ")";
	int xPosPrev2, yPosPrev2;


	if (s_Snake->xPosSnake < 0 || s_Snake->xPosSnake >= s_Snake->iWidth || s_Snake->yPosSnake < 0 || s_Snake->yPosSnake >= s_Snake->iHeight) {
		s_Snake->isDead = true;
	}

	/*for (int i = 1; i < s_Snake->tailLength; i++) {
		if (s_Snake->xTail[i] == s_Snake->xPosSnake && s_Snake->yTail[i] == s_Snake->yPosSnake) {
			s_Snake->isDead = true;
		}
	}*/

	/*for (int i = 0; i < s_Snake->tailLength; i++) {
		xPosPrev2 = s_Snake->xTail[i];
		yPosPrev2 = s_Snake->yTail[i];

		s_Snake->xTail[i] = xPosPrev;
		s_Snake->yTail[i] = yPosPrev;

		xPosPrev = xPosPrev2;
		yPosPrev = yPosPrev2;
	}*/

	std::thread SnakeIndexCreater(SnakeIndexCreaterThread, s_Snake);

	if (s_Snake->xFruit == s_Snake->xPosSnake && s_Snake->yFruit == s_Snake->yPosSnake) {
		s_Snake->iScore += 1;
		s_Snake->xFruit = rand() % (s_Snake->iWidth - 1) + 1;
		s_Snake->yFruit = rand() % (s_Snake->iHeight - 1) + 1;
		if (s_Snake->tailLength < s_Snake->maxTailLength) {
			s_Snake->tailLength += 1;
		}
		else {
			s_Snake->iScore += 1;
		}
	}

	if (s_Snake->iMoveDirection == 0) {
		s_Snake->yPosSnake -= 1;
	}
	else if (s_Snake->iMoveDirection == 1) {
		s_Snake->xPosSnake -= 1;
	}
	else if (s_Snake->iMoveDirection == 2) {
		s_Snake->yPosSnake += 1;
	}
	else if (s_Snake->iMoveDirection == 3) {
		s_Snake->xPosSnake += 1;
	}

	SnakeIndexCreater.join();
}

void Input(SSnake* s_Snake) {

	if (GetAsyncKeyState(0x57)) {			// W
		s_Snake->iMoveDirection = 0;
	}
	else if (GetAsyncKeyState(0x41)) {		// A
		s_Snake->iMoveDirection = 1;
	}
	else if (GetAsyncKeyState(0x53)) {		// S
		s_Snake->iMoveDirection = 2;
	}
	else if (GetAsyncKeyState(0x44)) {		// D
		s_Snake->iMoveDirection = 3;
	}
}


/*
TEST CODE!!!!
*/

void SnakeReset(SSnake* s_Snake) {
	s_Snake->iMoveDirection = 0;
	s_Snake->iScore = 0;
	s_Snake->isDead = false;
	s_Snake->isDead = false;
	s_Snake->tailLength = 1;
	s_Snake->xFruit = rand() % (s_Snake->iWidth - 1) + 1;
	s_Snake->yFruit = rand() % (s_Snake->iHeight - 1) + 1;
	s_Snake->xPosSnake = s_Snake->iWidth / 2;
	s_Snake->yPosSnake = s_Snake->iHeight / 2;

}




/*
TEST CODE!!!!
*/


void CheckSnakeIndexThread(SSnake* s_Snake) {
	for (int i = 1; i < s_Snake->tailLength; i++) {
		if (s_Snake->xTail[i] == s_Snake->xPosSnake && s_Snake->yTail[i] == s_Snake->yPosSnake) {
			s_Snake->isDead = true;
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void InitSnakeArray(SSnake* s_Snake) {
	s_Snake->xTail[0] = s_Snake->xPosSnake;
	s_Snake->yTail[0] = s_Snake->yPosSnake;

	for (int i = 1; i < s_Snake->tailLength; i++) {
		s_Snake->xTail[i] = i;
		s_Snake->yTail[i] = i;
	}
}

void ClearWindow() {
	system("cls");
}

void Test(SSnake* s_SnakeOG) {
	SSnake s_Snake1;
	SSnake* s_Snake2 = s_SnakeOG;
	SSnake* s_Snake3 = new SSnake;

	std::cout << "\nOG Snake(" << s_SnakeOG->xPosSnake << "," << s_SnakeOG->yPosSnake << ")\n";

	std::cout << "Snake One(" << s_Snake1.xPosSnake << "," << s_Snake1.yPosSnake << ")\n";
	std::cout << "Snake Two(" << s_Snake2->xPosSnake << "," << s_Snake2->yPosSnake << ")\n";
	std::cout << "Snake Thr(" << s_Snake3->xPosSnake << "," << s_Snake3->yPosSnake << ")\n";

	std::cout << "_______________________________________________________________________\n";
}

void SnakeIndexCreaterThread(SSnake* s_Snake) {
	int xPosPrev = s_Snake->xTail[0];
	int yPosPrev = s_Snake->yTail[0];
	//std::cout << "("<< xPosPrev << "," << yPosPrev << ")";
	int xPosPrev2, yPosPrev2;

	for (int i = 0; i < s_Snake->tailLength; i++) {
		xPosPrev2 = s_Snake->xTail[i];
		yPosPrev2 = s_Snake->yTail[i];

		s_Snake->xTail[i] = xPosPrev;
		s_Snake->yTail[i] = yPosPrev;

		xPosPrev = xPosPrev2;
		yPosPrev = yPosPrev2;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void externDrawing(SSnake* s_Snake) {
	for (int i = 0; i < s_Snake->iWidth + 2; i++) {
		std::cout << "#";
	}

	for (int i = 0; i < s_Snake->iHeight; i++) {
		std::cout << "\n#";
		//std::cout << "#";

		for (int j = 0; j < s_Snake->iWidth; j++) {
			
			if (s_Snake->xPosSnake == j && s_Snake->yPosSnake == i) {
				std::cout << "O";
			}
			else if (s_Snake->xFruit == j && s_Snake->yFruit == i) {
				std::cout << "F";
			}
			else {
				s_Snake->isDrawn = false;
				for (int k = 0; k < s_Snake->maxTailLength; k++) {

					if (s_Snake->xTail[k] == j && s_Snake->yTail[k] == i) {
						std::cout << "o";
						s_Snake->isDrawn = true;
					}
				}
				if (!s_Snake->isDrawn) {
					std::cout << " ";
				}
			}
		}
		std::cout << "#";
	}

	for (int i = 0; i < s_Snake->iWidth; i++) {
		std::cout << "\n#";

		for (int j = 0; j < s_Snake->iHeight; j++) {
			std::cout << " ";
		}

		std::cout << "#";
	}

	std::cout << "\n";
	for (int i = 0; i < s_Snake->iWidth + 2; i++) {
		std::cout << "#";
	}
}


//void Draw(int*, int*, int*, int*, int*, int*, const int*, int*, int*);
//
//void Logic(bool*, int* ,int* ,int*, int*, int*, int*, int*, int*, const int*, int* ,int*);
//
//int Input(int*);
//
//void GameOver(int*);


//int main() {
//	srand(time(NULL));
//
//	int iWidth = 20;
//	int iHeight = 20;
//	int iMove = 0;
//	int xPosSnake = iWidth / 2;
//	int yPosSnake = iHeight / 2;
//	int iScore = 0;
//
//	int xFruit = rand() % (iWidth - 1) + 1;
//	int yFruit = rand() % (iHeight - 1) + 1;
//
//	int const tailLength = 20;
//
//	int xTail[tailLength];
//	int yTail[tailLength];
//
//
//	bool isOver = false;
//
//	while (true) {
//		xTail[0] = xPosSnake;
//		yTail[0] = yPosSnake;
//
//		iMove = Input(&iMove);
//		Draw(&iWidth, &iHeight, &xPosSnake, &yPosSnake, &xFruit, &yFruit, &tailLength, xTail, yTail);
//		iMove = Input(&iMove);
//		Logic(&isOver, &iWidth, &iHeight, &xPosSnake, &yPosSnake, &iMove, &xFruit, &yFruit, &iScore, &tailLength, xTail, yTail);
//		iMove = Input(&iMove);
//
//		std::cout << "\nScore: " << iScore << "\n";
//		Sleep(75);
//
//		if (isOver) {
//			system("cls");
//
//			GameOver(&iScore);
//
//			isOver = false;
//			iMove = 0;
//			xPosSnake = iWidth / 2;
//			yPosSnake = iHeight / 2;
//			iScore = 0;
//			xFruit = rand() % (iWidth - 1) + 1;
//			yFruit = rand() % (iHeight - 1)+ 1;
//		}
//	}
//
//	std::cin.get();
//	return 0;
//}
//
//
//void Draw(int* _iWidth, int* _iHeight, int* _xPosSnake, int* _yPosSnake, int* _xFruit, int* _yFruit, const int* _tailLength, int* xTail, int* yTail) {
//	system("cls");
//
//	for (int i = 0; i < *_iWidth + 2; i++) {
//		std::cout << "#";
//	}
//
//	for (int k = 0; k < *_iHeight; k++) {
//		std::cout << "\n#";
//		for (int m = 0; m < *_iWidth; m++) {
//			if (*_xPosSnake == m && *_yPosSnake == k) {
//				std::cout << "O";
//			}
//			else if (*_xFruit == m && *_yFruit == k) {
//				std::cout << "F";
//			}
//			else {
//				bool isDrawn = false;
//
//				for (int i = 0; i < *_tailLength; i++) {
//					if (xTail[i] == m && yTail[i] == k) {
//						isDrawn = true;
//						std::cout << "o";
//					}
//				}
//				if (isDrawn == false) {
//					std::cout << " ";
//				}
//			}
//		}
//		std::cout << "#";
//	}
//	
//
//	std::cout << "\n";
//	for (int i = 0; i < *_iWidth + 2; i++) {
//		std::cout << "#";
//	}
//
//	std::cout << "\n";
//
//	for (int i = 0; i < *_tailLength; i++) {
//		std::cout << "Coords for " << i << " (" << xTail[i] << "," << yTail[i] << ")\n";
//	}
//	std::cout << "\n\n\nFruitPosition = (" << *_xFruit << "," << *_yFruit << ")";
//}
//
//void Logic(bool* isOver, int* _iWidth, int* _iHeight, int* _xPosSnake, int* _yPosSnake, int* _iMove, int* _xFruit, int* _yFruit , int* _iScore,const int* _tailLength ,int* _xTail, int* _yTail) {
//	int prevX_tail = _xTail[0];
//	int prevY_tail = _yTail[0];
//
//	int prevX2, prevY2;
//
//
//	if (*_xPosSnake < 0 || *_xPosSnake >= *_iWidth || *_yPosSnake < 0 || *_yPosSnake >= *_iHeight) {
//		*isOver = true;
//	}
//	else {
//		*isOver = false;
//	}
//
//	if (*_xFruit == *_xPosSnake && *_yPosSnake == *_yFruit) {
//		*_iScore += 1;
//		*_xFruit = rand() % (*_iWidth - 1) + 1;
//		*_yFruit = rand() % (*_iHeight - 1) + 1;
//
//		for (int g = 0; g < *_tailLength; g++) {
//			prevX2 = _xTail[g];
//			prevY2 = _yTail[g];
//
//			_xTail[g] = prevX_tail;
//			_yTail[g] = prevY_tail;
//
//			prevX_tail = prevX2;
//			prevY_tail = prevY2;
//		}
//		//*_tailLength += 1;
//	}
//
//	if (*_iMove == 0) {
//		*_yPosSnake -= 1;
//	}
//	else if (*_iMove == 1) {
//		*_xPosSnake -= 1;
//	}
//	else if (*_iMove == 2) {
//		*_yPosSnake += 1;
//	}
//	else if (*_iMove == 3) {
//		*_xPosSnake += 1;
//	}
//
//}
//
//int Input(int* _lastDirection) {
//	bool bStop = false;
//	if (GetAsyncKeyState(VK_NUMPAD9) & 1) {
//		bStop = true;
//	}
//
//	while (bStop) {
//		if (GetAsyncKeyState(VK_NUMPAD9) & 1) {
//			bStop = false;
//			break;
//		}
//	}
//
//	if (GetAsyncKeyState(0x57)) {		// W
//		return 0;
//	}
//	if (GetAsyncKeyState(0x41)) {		// A
//		return 1;
//	}
//	if (GetAsyncKeyState(0x53)) {		// S
//		return 2;
//	}
//	if (GetAsyncKeyState(0x44)) {		// D
//		return 3;
//	}
//	else {
//		return *_lastDirection;
//	}
//}
//
//void GameOver(int* _iScore) {
//	std::cout << "THE GAME IS OVER!\n";
//	Sleep(1000);
//	std::cout << "\nThe Game will restart in 3 seconds!\nScore: " << *_iScore;
//	Sleep(1000);
//	system("cls");
//	std::cout << "THE GAME IS OVER!\n";
//	std::cout << "\nThe Game will restart in 2 seconds!\nScore: " << *_iScore;
//	Sleep(1000);
//	system("cls");
//	std::cout << "THE GAME IS OVER!\n";
//	std::cout << "\nThe Game will restart in 1 seconds!\nScore: " << *_iScore;
//	Sleep(1000);
//	system("cls");
//}