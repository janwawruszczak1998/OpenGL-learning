/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi 
//  układu współrzędnych dla rzutowania perspektywicznego

/*************************************************************************************/

#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

typedef float point3[3];

static GLfloat r = 15; 

static GLfloat viewer[]= {0.0, 0.0, r};
static GLfloat angle[]= {0.0, 1.0, 0.0};


static GLfloat theta = 0.0;   // kąt obrotu obiektu
static GLfloat phi = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy
                               // 0 - nie naciśnięto żadnego klawisza
                               // 1 - naciśnięty zostać lewy klawisz

static int x_pos_old=0;       // poprzednia pozycja kursora myszy
static int y_pos_old=0;

static int delta_x=0;        // różnica pomiędzy pozycją bieżącą
static int delta_y=0;        // i poprzednią kursora myszy

// inicjalizacja położenia obserwatora

/*************************************************************************************/

// Funkcja rysująca osie układu wspó?rz?dnych

 /*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)       
    {
        x_pos_old=x;         // przypisanie aktualnie odczytanej pozycji kursora
        y_pos_old=y;                     // jako pozycji poprzedniej
        status = 1;          // wcięnięty został lewy klawisz myszy
    } 
    else

        status = 0;          // nie został wcięnięty żaden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich
// zmiennych globalnych

void Motion( GLsizei x, GLsizei y )
{
   
    delta_x=x-x_pos_old;     // obliczenie różnicy położenia kursora myszy
    delta_y=y-y_pos_old;
    x_pos_old=x;            // podstawienie bieżącego położenia jako poprzednie
    y_pos_old=y;
    glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

void Axes(void)
{

point3  x_min = {-5.0, 0.0, 0.0};
point3  x_max = { 5.0, 0.0, 0.0};
// pocz?tek i koniec obrazu osi x

point3  y_min = {0.0, -5.0, 0.0};
point3  y_max = {0.0,  5.0, 0.0};
// pocz?tek i koniec obrazu osi y

point3  z_min = {0.0, 0.0, -5.0};
point3  z_max = {0.0, 0.0,  5.0};
//  pocz?tek i koniec obrazu osi y
glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
glBegin(GL_LINES); // rysowanie osi x
glVertex3fv(x_min);
glVertex3fv(x_max);
glEnd();

glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
glBegin(GL_LINES);  // rysowanie osi y

glVertex3fv(y_min);
glVertex3fv(y_max);                           
glEnd();

glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
glBegin(GL_LINES); // rysowanie osi z

glVertex3fv(z_min);
glVertex3fv(z_max);
glEnd();

}

/*************************************************************************************/

// Funkcja określająca co ma być rysowane (zawsze wywoływana, gdy trzeba
// przerysować scenę)

 

void RenderScene(void)
{

 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Czyszczenie okna aktualnym kolorem czyszczącym

glLoadIdentity();
// Czyszczenie macierzy bie??cej

gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 0.0, 0.0, angle[0], angle[1], angle[2]);
// Zdefiniowanie położenia obserwatora
Axes();
// Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej

if(status == 1)                     // jeśli lewy klawisz myszy wcięnięty
{
    theta += delta_x*pix2angle;    // modyfikacja kąta obrotu o kat proporcjonalny
    phi += delta_y*pix2angle;
}                                  // do różnicy położeń kursora myszy

glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
glRotatef(phi, 1.0, 0.0, 0.0);  //obrót obiektu o nowy kąt



glColor3f(1.0f, 1.0f, 1.0f);
// Ustawienie koloru rysowania na biały

glutWireTeapot(3.0);
// Narysowanie czajnika
glFlush();
// Przekazanie poleceń rysujących do wykonania
     glutSwapBuffers();

 

 }
/*************************************************************************************/

// Funkcja ustalająca stan renderowania

void keys(unsigned char key, int x, int y)
{
    if(key == 'a') viewer[0]++;
    if(key == 'd') viewer[0]--;

    if(key == 'w'){
        viewer[1]++;
    }
    if(key == 's') viewer[1]--;

    if(key == 'o') viewer[2] = std::min(viewer[2] + 1, r);
    
    if(key == 'p') {viewer[2] = std::max(viewer[2] - 1, -r);
        if(viewer[2] == 0) viewer[0] *= -1; viewer[0] *= -1;
    }
    RenderScene();
}

void MyInit(void)
{

glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// Kolor czyszczący (wypełnienia okna) ustawiono na czarny

}

/*************************************************************************************/


// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

 

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

    pix2angle = 360.0/(float)horizontal;  // przeliczenie pikseli na stopnie

    glMatrixMode(GL_PROJECTION);
    // Przełączenie macierzy bieżącej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bieżącej

    gluPerspective(70, 1.0, 1.0, 300.0);
    // Ustawienie parametrów dla rzutu perspektywicznego


    if(horizontal <= vertical)
        glViewport(0, (vertical-horizontal)/2, horizontal, horizontal);

    else
        glViewport((horizontal-vertical)/2, 0, vertical, vertical);
    // Ustawienie wielkości okna okna widoku (viewport) w zależności
    // relacji pomiędzy wysokością i szerokością okna

    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu  

    glLoadIdentity();
    // Czyszczenie macierzy bieżącej

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

 

int main(int argc, char *argv[])
{          

glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);

glutInitWindowSize(500, 500);

glutInit(&argc, argv);

glutCreateWindow("Rzutowanie perspektywiczne");
              
glutMouseFunc(Mouse);
// Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy
   
glutMotionFunc(Motion);
// Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy

glutDisplayFunc(RenderScene);
// Określenie, że funkcja RenderScene będzie funkcją zwrotną
// (callback function).  Będzie ona wywoływana za każdym razem
// gdy zajdzie potrzeba przerysowania okna
 

glutReshapeFunc(ChangeSize);
// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
// za zmiany rozmiaru okna                       
 

MyInit();
// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
// inicjalizacje konieczne  przed przystąpieniem do renderowania
glEnable(GL_DEPTH_TEST);
// Włączenie mechanizmu usuwania niewidocznych elementów sceny

glutKeyboardFunc(keys);

glutMainLoop();
// Funkcja uruchamia szkielet biblioteki GLUT

return 0;
}