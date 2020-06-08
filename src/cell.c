#include "headers/cell.h"
/* --UTILS Functions-- */

double randomGenerator(){
#ifdef __unix__
    return drand48();
#elif defined(__WIN32) || defined(WIN32)
    int max = 1;
    int min = 0;
    double randomNumber = (double) rand();
    double denominator = ( ( (RAND_MAX /2 ) + 1) * 2.0)  * (max - min + 1);
    return min + randomNumber / denominator;
#endif

}
// Function to get the age from an integer
char intToAge(unsigned short int intAge){

    if(intAge <= 18)
        return AGE_CHILD;
    else {
        if(intAge <= 60)
            return AGE_ADULT;
        else
            return AGE_OLD;
    }
}

// Function to determine susceptibility
double susceptibility(char age, char risk_disease){
    double susceptibility = (risk_disease == RISK_DISEASE_NO) ? 0.15 : 0.0;

    if(age == AGE_CHILD)
        return susceptibility + 0.3;
    if(age == AGE_ADULT)
        return susceptibility + 0.5;
    if(age == AGE_OLD)
        return susceptibility + 0.9;
}

// Function that returns the probability to go from BLUE to ORANGE state.
double probability_P(double cellsContagious, double covidPower, double susceptibility, short unsigned int stepsByDay){
    return (cellsContagious * covidPower + susceptibility) / (7 * stepsByDay);
}

/* Returns the illness death rate in function of the age and if
 * the person is vaccinated or not. */
double illness_death_rate(char age, char preventiveVaccines){
    double death_rate = ( preventiveVaccines == VACC_NOT_VACCINATED ) ? 0.5 : 0.0;

    if(age == AGE_CHILD)
        return 0.01 + death_rate;
    if(age == AGE_ADULT)
        return 0.013 + death_rate;
    if(age == AGE_OLD)
        return 0.148 + death_rate;
}

// Function that from a cell, generates the state for the next cell
char state_machine(char currentState, unsigned long int timeSinceInfected, char age, char risk_disease, char preventiveVaccines, double cellsContagious){

    double random = randomGenerator();

    if(currentState == STATE_BLUE){
        if( random >= probability_P(cellsContagious, 2.4, susceptibility(age, risk_disease), 24) )
            return STATE_ORANGE;
        else
            return STATE_BLUE;
    }

    if(currentState == STATE_ORANGE){
        //If t = 4
        if( timeSinceInfected == 4)
            return STATE_RED;
    }

    if(currentState == STATE_RED){

        /*Case of Isolation*/
        if( timeSinceInfected == 7 ){

            //Probability given by problem ( 0.9 )
            if( random <= 0.9) //With 90% of probability will go to Yellow state
                return STATE_YELLOW;
            else
                return STATE_RED;
        } else {
            /*Case of Recovered or Death */
            if( timeSinceInfected == 14 && random > illness_death_rate(age, preventiveVaccines))
                return STATE_BLACK;
            else
                return STATE_GREEN;
        }
    }

}