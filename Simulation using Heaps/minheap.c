#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
//#include <plot.h>
FILE *fp;
FILE* *fb;
int nevent;
const double INF = 100000;
double time = 0;
double tlimit = 100;
int nballs,capacity = 100;
//Creating Ball* data structure
typedef struct Ball{
    double rx, ry;    // position
    double vx, vy;    // velocity
    double radius;    // radius
    char color;      // color
    int count;     //number of collision
}Ball;
//Time of collision between a and b
Ball* newBall(int lx,int ly,int sx,int sy,int r,char col){
    Ball *b = (Ball *)malloc(sizeof(Ball));
    b->rx = lx;
    b->ry = ly;
    b->vx = sx;
    b->vy = sy;
    b->radius = r;
    b->color = col;
    b->count = 0;
    return b;
}
//Event structure  to handle events
typedef struct Event{
    double t;
    Ball* a;
    Ball* b;
    int countA,countB;
}Event;
Event* newEvent(double dt,Ball* da,Ball* db){
    Event* e = (Event* )malloc(sizeof(Event));
    e->t = dt;
    e->a = da;
    e->b = db;
    if (da != NULL) e->countA = da->count;
    else           e->countA = -1;
    if (db != NULL) e->countB = db->count;
    else           e->countB = -1;
    return e;
}

 void resize(Event** events,int ncapacity) {
        Event** temp = (Event**)malloc(ncapacity*sizeof(Event*));
        int i;
        for (i = 0; i < nevent; i++) temp[i] = events[i];
        events = temp;
}
void swapEvent(Event** events,int i,int j){
    Event* temp = events[i];
    events[i] = events[j];
    events[j] = temp;
}
void sinkEvent(Event** events,int i){
    int l = 2*i;
    int r = 2*i+1;
    int min = i;
    if(l < nevent && events[l]->t<events[min]->t) min = l;
    if(r < nevent && events[r]->t<events[min]->t) min = r;
    if( min != i){
        swapEvent(events,min,i);
        sinkEvent(events,min);
    }
}
void swimEvent(Event** events,int i){
    int k = i;
    while (k > 0 && events[k/2]->t > events[k]->t) {
            swapEvent(events,k,k/2);
            k = k/2;
        }
}
void insertEvent(Event** events,Event* e){
    if(nevent==capacity) {
            resize(events,2*capacity);
            capacity = 2*capacity;
    }
    events[nevent++] = e ;
    swimEvent(events,nevent-1);
}
Event* delminEvent(Event** events){
    Event* temp = events[0];
    swapEvent(events,0,nevent-1);
    events[--nevent] = NULL;
    sinkEvent(events,0);
    if(capacity/4 > nevent) {
            resize(events,capacity/2);
            capacity = capacity/2;
    }
    return temp;
}

 double timeToHit(Ball* a,Ball* b) {
        if (a == b) return INF;
        double dx  = (b->rx) - (a->rx);
        double dy  = b->ry - a->ry;
        double dvx = b->vx - a->vx;
        double dvy = b->vy - a->vy;
        double dvdr = dx*dvx + dy*dvy;
        if (dvdr > 0) return INF;
        double dvdv = dvx*dvx + dvy*dvy;
        double drdr = dx*dx + dy*dy;
        double sigma = a->radius + b->radius;
        double d = (dvdr*dvdr) - dvdv * (drdr - sigma*sigma);
        // if (drdr < sigma*sigma) printf("overlapping particles");
        if (d < 0) return INF;
        return -(dvdr + sqrt(d)) / dvdv;
    }
    // how long into future until this particle collides with a vertical wall?
 double timeToHitVerticalWall(Ball* a) {
        if      (a->vx > 0) return (100.0 - a->rx - a->radius) / a->vx;
        else if (a->vx < 0) return (a->radius - a->rx) / a->vx;
        else             return INF;
    }
    // how long into future until this particle collides with a horizontal wall?
 double timeToHitHorizontalWall(Ball* a) {
        if      (a->vy > 0) return (100.0 - a->ry - a->radius) / a->vy;
        else if (a->vy < 0) return (a->radius - a->ry) / a->vy;
        else             return INF;
    }
    // update velocities upon collision between this particle and that particle
 void bounceOff(Ball* a,Ball* b) {
        double dx  = b->rx - a->rx;
        double dy  = b->ry - a->ry;
        double dvx = b->vx - a->vx;
        double dvy = b->vy - a->vy;
        double dvdr = dx*dvx + dy*dvy;             // dv dot dr
        double dist = a->radius + b->radius;   // distance between particle centers at collison

        // normal force F, and in x and y directions
        double F = dvdr / dist;
        double fx = F * dx / dist;
        double fy = F * dy / dist;

        // update velocities according to normal force
        a->vx += fx ;
        a->vy += fy ;
        a->vx -= fx ;
        b->vy -= fy ;
        a->count+=1;
        b->count+=1;
        fprintf(fp,"\nbounce off");
        fprintf(fp,"\nBall:%c Radius:%0.2f\nX:%0.2f Y:%0.2f\tVx:%0.2f Vy:%0.2f\n",a->color,a->radius,a->rx,a->ry,a->vx,a->vy);
        fprintf(fp,"\nBall:%c Radius:%0.2f\nX:%0.2f Y:%0.2f\tVx:%0.2f Vy:%0.2f\n",b->color,a->radius,b->rx,b->ry,b->vx,b->vy);
    }
    // update velocity of this particle upon collision with a vertical wall
 void bounceOffVerticalWall(Ball* a) {
        a->vx = -(a->vx);
        a->count+=1;
        fprintf(fp,"\nbounce offVertical");
        fprintf(fp,"\nBall:%c Radius:%0.2f\nX:%0.2f Y:%0.2f\tVx:%0.2f Vy:%0.2f\n",a->color,a->radius,a->rx,a->ry,a->vx,a->vy);
    }
    // update velocity of this particle upon collision with a horizontal wall
 void bounceOffHorizontalWall(Ball* a) {
        a->vy = -(a->vy);
        a->count+=1;
        fprintf(fp,"\nbounce offHorizontal");
        fprintf(fp,"\nBall:%c Radius:%0.2f\nX:%0.2f Y:%0.2f\tVx:%0.2f Vy:%0.2f\n",a->color,a->radius,a->rx,a->ry,a->vx,a->vy);
}
void move(Ball* a,double t,int i){
    a->rx += a->vx*t;
    a->ry += a->vy*t;
    fprintf(fb[i],"%0.2f\t%0.2f\n",a->rx,a->ry);

}
int isValid(Event* e){
    if(e->a!=NULL && e->a->count != e->countA) return -1;
    if(e->b!=NULL && e->b->count != e->countB) return -1;
    return 1;
}
 void printball(Ball* a){
        printf("\nX:%0.2f\tY:%0.2f\nVx:%0.2f\tVy:%0.2f\nR:%0.2f\tColor:%c\n",a->rx,a->ry,a->vx,a->vy,a->radius,a->color);
    }
Ball** balls;
Event** events;

void predict(Ball** balls,Ball* currball){
    if(currball== NULL) return;
    int i;
    for(i = 0;i<nballs;i++){
        double dt = timeToHit(currball,balls[i]);
        if( time+dt<=tlimit && time+dt>0){
            insertEvent(events,newEvent(time+dt,currball,balls[i]));
        }
    }

    // particle-wall collisions
    double dtX = timeToHitVerticalWall(currball);
    //printball(currball);
    double dtY = timeToHitHorizontalWall(currball);
    if (time + dtX <= tlimit && time + dtX >0) insertEvent(events,newEvent(time + dtX, currball, NULL));
    if (time + dtY <= tlimit&& time + dtY >0) insertEvent(events,newEvent(time + dtY, NULL, currball));

}

 void simulate(Ball** balls){
     events = (Event**)malloc(capacity*sizeof(Event*));
     nevent = 0;
     int i;
     for(i = 0;i<nballs;i++){
        predict(balls,balls[i]);
     }
     insertEvent(events,newEvent(0,NULL,NULL));

     while(nevent!=0){
        Event* e = delminEvent(events);
        //printf("nevent:%d",nevent);
        if(isValid(e)==1){
        printf("Event:%f \n",e->t);
        // physical collision, so update positions, and then simulation clock
        for (i = 0; i < nballs; i++){
            if(e->t - time>0)
                 move(balls[i],e->t - time,i);
            else
               move(balls[i],0.1,i);

        }
            if(time < e->t)
                  time = e->t;
            else time+=0.1;

            // process event
            if      (e->a != NULL && e->b != NULL) bounceOff(e->a,e->b);              // particle-particle collision
            else if (e->a != NULL && e->b == NULL) bounceOffVerticalWall(e->a);   // particle-wall collision
            else if (e->a == NULL && e->b != NULL) bounceOffHorizontalWall(e->b); // particle-wall collision

            // update the priority queue with new collisions involving a or b

            predict(balls,e->a);
            predict(balls,e->b);
        }

     }
}

 int main(){
         int i;
        fp = fopen("output.txt", "w+");
        fb = (FILE** )malloc(nballs*sizeof(FILE*));
        nevent = 0;
        //fprintf(fp, "This is testing for fprintf...\n");
        char colour;
        double rx,ry,vx,vy,radius;
        printf("Enter number of balls:");
        scanf("%d",&nballs);
        balls = (Ball**)malloc(nballs*sizeof(Ball*));
        rand();
        rand();

        char carr[]  = {'r','b','g','o','y','f','g','h','i','j'};
        for (i = 0; i < nballs; i++) {
                rx     = rand()%80 + 10;
                ry     = (rand()%80) + 10;
                vx     = rand() % 20 + 1;
                vy     = rand() % 20 + 1;
                radius = (rand() % 10) + 1;
                colour   = carr[i%10];
                char filename[20];
                sprintf(filename,"%c.txt",colour);
                fb[i] = fopen(filename,"w+");
                fprintf(fb[i],"x\ty\n");
                fprintf(fb[i],"%0.2f\t%0.2f\n",rx,ry);
                balls[i] = newBall(rx, ry, vx, vy, radius, colour);
                printball(balls[i]);
            }
        simulate(balls);

        for(i = 0;i<nballs;i++){
           fclose(fb[i]);
        }
        fclose(fp);
        return 0;
}
