#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 25
#define COLS 60
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef struct {
    int id;
    int type;      // 1=Rectangle, 2=Line, 3=Circle, 4=Triangle
    int p[8];
    int active;
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

/* ---------- Canvas Functions ---------- */

void initCanvas() {
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++)
            canvas[i][j] = '_';
}

void displayCanvas() {
    printf("\n");
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++)
            printf("%c ", canvas[i][j]);
        printf("\n");
    }
}

void setPixel(int x, int y) {
    if(x >= 0 && x < ROWS && y >= 0 && y < COLS)
        canvas[x][y] = '*';
}

/* ---------- Drawing Functions ---------- */

void drawLineCanvas(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1) {
        setPixel(x1, y1);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangleCanvas(int x, int y, int width, int height) {

    for(int i = y; i < y + width; i++) {
        setPixel(x, i);
        setPixel(x + height - 1, i);
    }

    for(int i = x; i < x + height; i++) {
        setPixel(i, y);
        setPixel(i, y + width - 1);
    }
}

void drawCircleCanvas(int xc, int yc, int r) {

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while(x <= y) {

        setPixel(xc + x, yc + y);
        setPixel(xc - x, yc + y);
        setPixel(xc + x, yc - y);
        setPixel(xc - x, yc - y);

        setPixel(xc + y, yc + x);
        setPixel(xc - y, yc + x);
        setPixel(xc + y, yc - x);
        setPixel(xc - y, yc - x);

        if(d < 0)
            d += 4 * x + 6;
        else {
            d += 4 * (x - y) + 10;
            y--;
        }

        x++;
    }
}

void drawTriangleCanvas(int x1, int y1,
                        int x2, int y2,
                        int x3, int y3) {

    drawLineCanvas(x1, y1, x2, y2);
    drawLineCanvas(x2, y2, x3, y3);
    drawLineCanvas(x3, y3, x1, y1);
}

/* ---------- Redraw Entire Canvas ---------- */

void redrawCanvas() {

    initCanvas();

    for(int i = 0; i < objectCount; i++) {

        if(!objects[i].active)
            continue;

        switch(objects[i].type) {

            case 1:
                drawRectangleCanvas(
                    objects[i].p[0],
                    objects[i].p[1],
                    objects[i].p[2],
                    objects[i].p[3]
                );
                break;

            case 2:
                drawLineCanvas(
                    objects[i].p[0],
                    objects[i].p[1],
                    objects[i].p[2],
                    objects[i].p[3]
                );
                break;

            case 3:
                drawCircleCanvas(
                    objects[i].p[0],
                    objects[i].p[1],
                    objects[i].p[2]
                );
                break;

            case 4:
                drawTriangleCanvas(
                    objects[i].p[0],
                    objects[i].p[1],
                    objects[i].p[2],
                    objects[i].p[3],
                    objects[i].p[4],
                    objects[i].p[5]
                );
                break;
        }
    }
}

/* ---------- Object Operations ---------- */

void addRectangle() {

    Object obj;

    obj.id = objectCount + 1;
    obj.type = 1;
    obj.active = 1;

    printf("Enter x y width height: ");
    scanf("%d%d%d%d",
          &obj.p[0], &obj.p[1],
          &obj.p[2], &obj.p[3]);

    objects[objectCount++] = obj;

    redrawCanvas();
}

void addLine() {

    Object obj;

    obj.id = objectCount + 1;
    obj.type = 2;
    obj.active = 1;

    printf("Enter x1 y1 x2 y2: ");
    scanf("%d%d%d%d",
          &obj.p[0], &obj.p[1],
          &obj.p[2], &obj.p[3]);

    objects[objectCount++] = obj;

    redrawCanvas();
}

void addCircle() {

    Object obj;

    obj.id = objectCount + 1;
    obj.type = 3;
    obj.active = 1;

    printf("Enter centerX centerY radius: ");
    scanf("%d%d%d",
          &obj.p[0], &obj.p[1],
          &obj.p[2]);

    objects[objectCount++] = obj;

    redrawCanvas();
}

void addTriangle() {

    Object obj;

    obj.id = objectCount + 1;
    obj.type = 4;
    obj.active = 1;

    printf("Enter x1 y1 x2 y2 x3 y3: ");
    scanf("%d%d%d%d%d%d",
          &obj.p[0], &obj.p[1],
          &obj.p[2], &obj.p[3],
          &obj.p[4], &obj.p[5]);

    objects[objectCount++] = obj;

    redrawCanvas();
}

void listObjects() {

    printf("\nObjects:\n");

    for(int i = 0; i < objectCount; i++) {

        if(objects[i].active)
            printf("ID = %d  Type = %d\n",
                   objects[i].id,
                   objects[i].type);
    }
}

void deleteObject() {

    int id;

    listObjects();

    printf("Enter Object ID to delete: ");
    scanf("%d", &id);

    for(int i = 0; i < objectCount; i++) {

        if(objects[i].id == id &&
           objects[i].active) {

            objects[i].active = 0;

            redrawCanvas();

            printf("Deleted.\n");
            return;
        }
    }

    printf("Object not found.\n");
}

void modifyObject() {

    int id;

    listObjects();

    printf("Enter Object ID to modify: ");
    scanf("%d", &id);

    for(int i = 0; i < objectCount; i++) {

        if(objects[i].id == id &&
           objects[i].active) {

            switch(objects[i].type) {

                case 1:
                    printf("Enter new x y width height: ");
                    scanf("%d%d%d%d",
                          &objects[i].p[0],
                          &objects[i].p[1],
                          &objects[i].p[2],
                          &objects[i].p[3]);
                    break;

                case 2:
                    printf("Enter new x1 y1 x2 y2: ");
                    scanf("%d%d%d%d",
                          &objects[i].p[0],
                          &objects[i].p[1],
                          &objects[i].p[2],
                          &objects[i].p[3]);
                    break;

                case 3:
                    printf("Enter new centerX centerY radius: ");
                    scanf("%d%d%d",
                          &objects[i].p[0],
                          &objects[i].p[1],
                          &objects[i].p[2]);
                    break;

                case 4:
                    printf("Enter new x1 y1 x2 y2 x3 y3: ");
                    scanf("%d%d%d%d%d%d",
                          &objects[i].p[0],
                          &objects[i].p[1],
                          &objects[i].p[2],
                          &objects[i].p[3],
                          &objects[i].p[4],
                          &objects[i].p[5]);
                    break;
            }

            redrawCanvas();

            printf("Modified.\n");
            return;
        }
    }

    printf("Object not found.\n");
}

/* ---------- Main ---------- */

int main() {

    int choice;

    initCanvas();

    while(1) {

        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Rectangle\n");
        printf("2. Add Line\n");
        printf("3. Add Circle\n");
        printf("4. Add Triangle\n");
        printf("5. Delete Object\n");
        printf("6. Modify Object\n");
        printf("7. Display Picture\n");
        printf("8. List Objects\n");
        printf("9. Exit\n");
        printf("Enter Choice: ");

        scanf("%d", &choice);

        switch(choice) {

            case 1: addRectangle(); break;
            case 2: addLine(); break;
            case 3: addCircle(); break;
            case 4: addTriangle(); break;
            case 5: deleteObject(); break;
            case 6: modifyObject(); break;
            case 7: displayCanvas(); break;
            case 8: listObjects(); break;
            case 9: return 0;

            default:
                printf("Invalid Choice!\n");
        }
    }
}