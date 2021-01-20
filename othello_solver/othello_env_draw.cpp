#include "othello_env.h"

void Env::draw()
{
    for (int i = 0; i < szboard; i++) {
        for (int j = 0; j < szboard; j++) {
            switch (board[i][j]) {
                case piece::EMPTY:
                    printf(". ");
                    break;
                case piece::BLACK:
                    printf("¡Û");
                    break;
                case piece::WHITE:
                    printf("¡Ü");
                    break;
                case piece::OBSTACLE:
                    printf("¡Ø");
                    break;
                default:
                    printf("  ");
                    break;
            }
        }
        printf("\n");
    }
}

void Env::drawPlaceable()
{
    std::vector<std::pair<int, int>> placeable = calcPlaceable();
    std::vector<std::pair<int, int>>::iterator iPlaceable = placeable.begin();
    for (int i = 0; i < szboard; i++) {
        for (int j = 0; j < szboard; j++) {
            switch (board[i][j]) {
            case piece::EMPTY:
                if (iPlaceable != placeable.end() && std::make_pair(i, j) == *iPlaceable) {
                    if (isBlackTurn()) printf("¡Þ");
                    else printf("¡ß");
                    iPlaceable++;
                    break;
                }
                else {
                    printf("¡à");
                    break;
                }

            case piece::BLACK:
                printf("¡Û");
                break;
            case piece::WHITE:
                printf("¡Ü");
                break;
            case piece::OBSTACLE:
                printf("¡Ø");
                break;
            default:
                printf("  ");
                break;

            }
        }
        printf("\n");
    }
}