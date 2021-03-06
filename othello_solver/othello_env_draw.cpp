#include "othello_env.h"

void Env::draw() const
{
    printf("  0 1 2 3 4 5 6 7 \n");
    for (int i = 0; i < szboard; i++) {
        printf("%d ", i);
        for (int j = 0; j < szboard; j++) {
            switch (board[i][j]) {
                case piece::EMPTY:
                    printf(". ");
                    break;
                case piece::BLACK:
                    printf("��");
                    break;
                case piece::WHITE:
                    printf("��");
                    break;
                case piece::OBSTACLE:
                    printf("��");
                    break;
                default:
                    printf("  ");
                    break;
            }
        }
        printf("\n");
    }
}

void Env::drawPlaceable() const
{
    std::vector<std::pair<int, int>> placeable = calcPlaceable();
    std::vector<std::pair<int, int>>::iterator iPlaceable = placeable.begin();
    for (int i = 0; i < szboard; i++) {
        for (int j = 0; j < szboard; j++) {
            switch (board[i][j]) {
            case piece::EMPTY:
                if (iPlaceable != placeable.end() && std::make_pair(i, j) == *iPlaceable) {
                    if (isBlackTurn()) printf("��");
                    else printf("��");
                    iPlaceable++;
                    break;
                }
                else {
                    printf("��");
                    break;
                }

            case piece::BLACK:
                printf("��");
                break;
            case piece::WHITE:
                printf("��");
                break;
            case piece::OBSTACLE:
                printf("��");
                break;
            default:
                printf("  ");
                break;

            }
        }
        printf("\n");
    }
}