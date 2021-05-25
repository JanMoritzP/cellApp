#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 1000
#define HEIGHT 1000

typedef struct {
    unsigned short xPos;
    unsigned short yPos;
    unsigned short age;
    unsigned short maxAge;
    unsigned short health;
    unsigned short maxHealth;
    unsigned short dmg;
    int r;
    int g;
    int b;
    float infectivity;
    bool isAlive;
    bool isReserved;
} cell; 

void initGrid(cell* grid) {
    for(int i = 0; i < WIDTH*HEIGHT; i++) {
        (grid + i)->isAlive = false;
        (grid + i)->isReserved = false;
        (grid + i)->age = 0;
        (grid + i)->maxAge = 0;
        (grid + i)->xPos = i%WIDTH;
        (grid + i)->yPos = i/HEIGHT;
        (grid + i)->r = 0;
        (grid + i)->g = 0;
        (grid + i)->b = 0;
        //if(i%WIDTH > 200 && i%WIDTH < 400 && i/HEIGHT > 100 && i/HEIGHT < 300) (grid + i)->isReserved = true;
    }
}

bool checkRGBValues(cell* one, cell* two) {
    if(one->r != two->r) return false;
    if(one->g != two->g) return false;
    if(one->b != two->b) return false;
    return true;
}

void ageCells(cell* grid) {
    for(int i = 0; i < WIDTH*HEIGHT; i++) {
        if((grid + i)->isAlive) {
            (grid + i)->age++;
            /* int counter = 0;
            if((grid + i%WIDTH + HEIGHT*(i/HEIGHT))->xPos < WIDTH) {
                if((grid + (i + 1)%WIDTH + HEIGHT*(i/HEIGHT))->isAlive)  counter++;                
            }
            if((grid + i%WIDTH + HEIGHT*(i/HEIGHT))->xPos > 0) {
                if((grid + (i - 1)%WIDTH + HEIGHT*(i/HEIGHT))->isAlive)  counter++;                
            }
            if((grid + i%WIDTH + HEIGHT*(i/HEIGHT))->yPos < HEIGHT) {
                if((grid + i%WIDTH + HEIGHT*((i + 1)/HEIGHT))->isAlive)  counter++;                
            }
            if((grid + i%WIDTH + HEIGHT*(i/HEIGHT))->yPos > 0) {
                if((grid + i%WIDTH + HEIGHT*((i - 1)/HEIGHT))->isAlive)  counter++;                
            }           

            if(counter == 0 || counter == 4)  (grid + i)->age += 1;
            if(counter == 1 || counter == 3)  (grid + i)->age += 1;
            if(counter == 2)  {
                //(grid + i)->age = (grid + i)->maxAge + 1;
                (grid + i)->age++;
                //(grid + i)->age--;
                //if(!(grid + (i + 1)%WIDTH + HEIGHT*(i/HEIGHT))->isAlive)  (grid + (i + 1)%WIDTH + HEIGHT*(i/HEIGHT))->isReserved = true;
                //if(!(grid + (i - 1)%WIDTH + HEIGHT*(i/HEIGHT))->isAlive)  (grid + (i - 1)%WIDTH + HEIGHT*(i/HEIGHT))->isReserved = true;
                //if(!(grid + i%WIDTH + HEIGHT*((i + 1)/HEIGHT))->isAlive)  (grid + i%WIDTH + HEIGHT*((i + 1)/HEIGHT))->isReserved = true;
                //if(!(grid + i%WIDTH + HEIGHT*((i - 1)/HEIGHT))->isAlive)  (grid + i%WIDTH + HEIGHT*((i - 1)/HEIGHT))->isReserved = true;
            } */

        }
        if((grid + i)->age > (grid + i)->maxAge) {
            (grid + i)->isAlive = false;
            (grid + i)->r = 0;
            (grid + i)->g = 0;
            (grid + i)->b = 0;


            /* 
            if(!(grid + (i + 1)%WIDTH + HEIGHT*(i/HEIGHT))->isAlive)  (grid + (i + 1)%WIDTH + HEIGHT*(i/HEIGHT))->isReserved = false;
            if(!(grid + (i - 1)%WIDTH + HEIGHT*(i/HEIGHT))->isAlive)  (grid + (i - 1)%WIDTH + HEIGHT*(i/HEIGHT))->isReserved = false;
            if(!(grid + i%WIDTH + HEIGHT*((i + 1)/HEIGHT))->isAlive)  (grid + i%WIDTH + HEIGHT*((i + 1)/HEIGHT))->isReserved = false;
            if(!(grid + i%WIDTH + HEIGHT*((i - 1)/HEIGHT))->isAlive)  (grid + i%WIDTH + HEIGHT*((i - 1)/HEIGHT))->isReserved = false; */
        } 
        else if(1) {
            int ageSum = 0;
            unsigned short counter = 1;
            unsigned short xPos = (grid + i)->xPos;
            unsigned short yPos = (grid + i)->yPos;
            ageSum += (grid + i)->age;
            for(int x = -1; x <= 1; x++) {
                for(int y = -1; y <= 1; y++) {
                    if(x != 0 && y != 0) {
                        if(xPos + x < WIDTH && xPos + x >= 0 && yPos + y < HEIGHT && yPos + y >= 0) {
                            if(checkRGBValues((grid + i), (grid + (i + x)%WIDTH + HEIGHT*((i + y)/HEIGHT))) && (grid + (i + x)%WIDTH + HEIGHT*((i + y)/HEIGHT))->isAlive) {
                                ageSum += (grid + (i + x)%WIDTH + HEIGHT*((i + y)/HEIGHT))->age;
                                counter++;
                            }                            
                        }
                    }
                }
            }
            int ageAvg = (int) ((float) ageSum/(float) counter);
            if((float) ageAvg < (float) (grid + i)->age*0.5 || (float) ageAvg > (float) (grid + i)->age*1.5) {
                (grid + i)->age = ageAvg;
                for(int x = -1; x <= 1; x++) {
                    for(int y = -1; y <= 1; y++) {
                        if(x != 0 && y != 0) {
                            if(xPos + x < WIDTH && xPos + x >= 0 && yPos + y < HEIGHT && yPos + y >= 0) {
                                if(checkRGBValues((grid + i), (grid + (i + x)%WIDTH + HEIGHT*((i + y)/HEIGHT))) && (grid + (i + x)%WIDTH + HEIGHT*((i + y)/HEIGHT))->isAlive) {
                                    (grid + (i + x)%WIDTH + HEIGHT*((i + y)/HEIGHT))->age = ageAvg;
                                }                            
                            }
                        }
                    }
                }
            }
        }
    }
}

void addCellToGrid(cell* grid, cell newCell) {
    *(grid + HEIGHT*newCell.yPos + newCell.xPos) = newCell;
}

void savePicture(cell* grid, int iteration) {
    FILE* fp;    
    char name[20];
    snprintf(name, sizeof(name), "pic/%03d.ppm", (iteration-1)%100);
    fp = fopen(name, "w");
    char header[50];
    snprintf(header, sizeof(header), "P3 %d %d 255\n", WIDTH, HEIGHT);
    fprintf(fp, header);
    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            if((grid + x + y*HEIGHT)->isAlive) {
                if((grid + x + y*HEIGHT)->age > 0) {
                    char color[14];
                    float aging = ((float) (grid + x + y*HEIGHT)->maxAge - (float) (grid + x + y*HEIGHT)->age) / (float) (grid + x + y*HEIGHT)->maxAge;  //Aging = 0 when cell is old; 1 when cell is young
                    int rBright = ((float) (grid + x + y*HEIGHT)->r - 255) * aging + 255;
                    int gBright = ((float) (grid + x + y*HEIGHT)->g - 255) * aging + 255;
                    int bBright = ((float) (grid + x + y*HEIGHT)->b - 255) * aging + 255;
                    snprintf(color, sizeof(color), "%d %d %d\n", rBright, gBright, bBright);
                    fprintf(fp, color);
                }
                else {
                    fprintf(fp, "255 255 255\n");
                }
            }
            else fprintf(fp, "255 255 255\n");
            //if(y % 15 == 0) fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    if(iteration % 100 == 0) {
        char command[25];
        snprintf(command, sizeof(command), "./createVideo.sh %03d", iteration/100);
        system(command);
    }
}

void readConfigFile(cell* grid) {
    FILE* fp;
    fp = fopen("cells.config", "r");
    char buf[20];
    cell newCell;
    while(fscanf(fp, "%s", buf) != EOF) {
        if(strcmp(buf, "Infectivity:") == 0) {
            float infectivity;
            fscanf(fp, "%f", &infectivity);
            newCell.infectivity = infectivity;
        }
        else if(strcmp(buf, "maxAge:") == 0) {
            int maxAge;
            fscanf(fp, "%d", &maxAge);
            newCell.maxAge = maxAge;
            newCell.age = 0;
            newCell.isAlive = true;
            newCell.isReserved = false;
        }
        else if(strcmp(buf, "xPos:") == 0) {
            int xPos;
            fscanf(fp, "%d", &xPos);
            newCell.xPos = xPos;            
        }
        else if(strcmp(buf, "yPos:") == 0) {
            int yPos;
            fscanf(fp, "%d", &yPos);
            newCell.yPos = yPos;            
        }
        else if(strcmp(buf, "DMG:") == 0) {
            int dmg;
            fscanf(fp, "%d", &dmg);
            newCell.dmg = dmg;            
        }
        else if(strcmp(buf, "R:") == 0) {
            int r;
            fscanf(fp, "%d", &r);
            newCell.r = r;
        }
        else if(strcmp(buf, "G:") == 0) {
            int g;
            fscanf(fp, "%d", &g);
            newCell.g = g;
        }
        else if(strcmp(buf, "B:") == 0) {
            int b;
            fscanf(fp, "%d", &b);
            newCell.b = b;            
        }
        else if(strcmp(buf, "---") == 0) {
            addCellToGrid(grid, newCell);
        }
    }
}

void createChildCell(cell* grid, cell parentCell, int direction) {
    cell newCell;
    switch (direction)
    {
    case 1:  //TOP
        newCell.yPos = parentCell.yPos + 1;
        newCell.xPos = parentCell.xPos;
        break;
    case 2:  //RIGHT
        newCell.yPos = parentCell.yPos;
        newCell.xPos = parentCell.xPos + 1;
        break;
    case 3:  //BOTTOM
        newCell.yPos = parentCell.yPos - 1;
        newCell.xPos = parentCell.xPos;
        break;
    case 4:  //LEFT
        newCell.yPos = parentCell.yPos;
        newCell.xPos = parentCell.xPos - 1;
        break;
    default:
        break;
    }
    newCell.infectivity = parentCell.infectivity;
    newCell.age = 0;
    newCell.maxAge = parentCell.maxAge;
    newCell.isAlive = true;
    newCell.r = parentCell.r;
    newCell.g = parentCell.g;
    newCell.b = parentCell.b;
    newCell.dmg = parentCell.dmg;
    addCellToGrid(grid, newCell);    
}


void fightCellsAround(cell* grid, cell parentCell) {
    unsigned short xPos = parentCell.xPos;
    unsigned short yPos = parentCell.yPos;
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            if(x != 0 && y != 0) {
                if(xPos + x < WIDTH && xPos + x >= 0 && yPos + y < HEIGHT && yPos + y >= 0) {
                    if(!checkRGBValues(&parentCell, (grid + xPos + x + (yPos + y)*HEIGHT)) && (grid + xPos + x + (yPos + y)*HEIGHT)->isAlive) {
                        (grid + xPos + x + (yPos + y)*HEIGHT)->age += parentCell.dmg;
                    }
                }
            }
        }
    }
}

int main()
{
    system("./cleanPics.sh");
    cell* grid = malloc(sizeof(cell) * WIDTH * HEIGHT);
    initGrid(grid);
    
    readConfigFile(grid);
    int cellAmount = 1;
    int currentCellAmount = 0;
    int runs = 0;
    __time_t t;
    srand((unsigned) time(&t));
    while(runs < 3000 && cellAmount > 0) {
        currentCellAmount = 0;
        for(int i = 0; i < WIDTH*HEIGHT; i++) {
            if((grid + i)->isAlive) {
                currentCellAmount++;
                cell parentCell = *(grid + i%WIDTH + HEIGHT*(i/HEIGHT));
                if(parentCell.infectivity > (float) rand()/(float) RAND_MAX) {
                    float side = (float) rand()/(float) RAND_MAX;
                    if(side < 0.25) {  //TOP
                        if(parentCell.yPos + 1 < HEIGHT) {
                            if(!(grid + parentCell.xPos + (parentCell.yPos + 1)*HEIGHT)->isAlive && !(grid + parentCell.xPos + (parentCell.yPos + 1)*HEIGHT)->isReserved) {
                                createChildCell(grid, parentCell, 1);
                            }
                        }
                    }
                    else if(side < 0.5) {  //RIGHT
                        if(parentCell.xPos + 1 < WIDTH) {
                            if(!(grid + parentCell.xPos + 1 + parentCell.yPos*HEIGHT)->isAlive && !(grid + parentCell.xPos + 1 + parentCell.yPos*HEIGHT)->isReserved) {
                                createChildCell(grid, parentCell, 2);
                            }
                        }
                    }
                    else if(side < 0.75) {  //BOTTOM
                        if(parentCell.yPos - 1 >= 0) {
                            if(!(grid + parentCell.xPos + (parentCell.yPos - 1)*HEIGHT)->isAlive && !(grid + parentCell.xPos + (parentCell.yPos - 1)*HEIGHT)->isReserved) {
                                createChildCell(grid, parentCell, 3);
                            }
                        }
                    }
                    else if(side <= 1) {  //LEFT
                        if(parentCell.xPos - 1 >= 0) {
                            if(!(grid + parentCell.xPos - 1 + parentCell.yPos*HEIGHT)->isAlive && !(grid + parentCell.xPos - 1 + parentCell.yPos*HEIGHT)->isReserved) {
                                createChildCell(grid, parentCell, 4);
                            }
                        }
                    }
                }
                // FIGHT OTHER CELLS HERE
                fightCellsAround(grid, parentCell);
            }
        }
        cellAmount = currentCellAmount;
        ageCells(grid);
        runs++;
        savePicture(grid, runs);
    }

    system("./createMergedAvi.sh");
    printf("Done with %d runs\n", runs);
    free(grid);
    
    return 0;
}

