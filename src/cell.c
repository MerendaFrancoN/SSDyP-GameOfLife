#include "headers/cell.h"

/* --UTILS Functions-- */
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

    double random = randomDoubleGenerator();

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

//Function to create a cell with some configuration
cell_type createRandomCell(double childRate, double adultRate, double oldRate, double infectionRate){

    cell_type cellToReturn;

    //Generate Random Age
    double randomValue = randomDoubleGenerator();
    if( randomValue <= childRate)
        cellToReturn.age = AGE_CHILD;
    else{
        if(randomValue <= childRate + adultRate)
            cellToReturn.age = AGE_ADULT;
        else
            cellToReturn.age = AGE_OLD;
    }

    //Generate random Risk Disease
    cellToReturn.risk_disease = randomEnumIntGenerator(-1, RISK_DISEASES_NUMBER);

    //Generate Random Risk Professions
    cellToReturn.risk_professions = ( cellToReturn.age == AGE_CHILD ) ?
                                    RISK_PROF_NO :
                                    randomEnumIntGenerator(-1, RISK_PROFESSIONS_NUMBER);

    //Generate Random Preventive Vaccines
    cellToReturn.preventive_vaccines = randomEnumIntGenerator(-1, PREVENTIVE_VACCINES_NUMBER);
    //Generate Random Sex
    cellToReturn.biological_sex = randomEnumIntGenerator(0, 2);

    //Generate Random State
    randomValue = randomDoubleGenerator();
    cellToReturn.state = (randomValue <= infectionRate) ? STATE_WHITE : STATE_ORANGE;
    cellToReturn.timeSinceInfected = 0;

    //Return cell
    return cellToReturn;
}

//Function to print a Cell
void printCell(cell_type cell){
    printf("{\n");

    //Print Age
    printf("\tage: ");
    switch (cell.age) {
        case AGE_CHILD:
            printf("CHILD\n");
            break;
        case AGE_ADULT:
            printf("ADULT\n");
            break;
        case AGE_OLD:
            printf("OLD\n");
            break;
    }

    //Print Sex
    printf("\tbiological_sex: ");
    cell.biological_sex == SEX_MAN ? printf("MAN \n") : printf("WOMAN\n");

    //Print disease
    printf("\tdisease: ");
    switch (cell.risk_disease) {
        case RISK_DISEASE_NO:
            printf("NO DISEASE\n");
            break;
        case RISK_DISEASE_DIABETES:
            printf("DIABETES\n");
            break;
        case RISK_DISEASE_HYPERTENSION:
            printf("HYPERTENSION\n");
            break;
        case RISK_DISEASE_IMMUNODEPRESSED:
            printf("IMMUNODEPRESSED\n");
            break;
        case RISK_DISEASE_RESPIRATORY_DISEASE:
            printf("RESPIRATORY DISEASE\n");
            break;
    }

    //Print Profession
    printf("\tprofession: ");
    switch (cell.risk_professions) {
        case RISK_PROF_NO:
            printf("NO PROF\n");
            break;
        case RISK_PROF_DENTIST:
            printf("DENTIST\n");
            break;
        case RISK_PROF_ENFERMERY:
            printf("ENFERMERY\n");
            break;
        case RISK_PROF_PUBLIC_CHAUFFEUR:
            printf("PUBLIC_CHAUFFEUR\n");
            break;
    }

    //Print Vaccines
    printf("\tvaccines: ");
    switch (cell.preventive_vaccines) {
        case VACC_NOT_VACCINATED:
            printf("NOT VACC\n");
            break;
        case VACC_FLU:
            printf("FLU\n");
            break;
        case VACC_PNEUMONIA:
            printf("PNEUMONIA\n");
            break;
        case VACC_MENINGITIS:
            printf("MENINGITIS\n");
            break;
    }

    //Print State and Time since Infection
    printf("\tstate: ");
    switch (cell.state) {
        case STATE_WHITE:
            printf("WHITE\n");
            break;
        case STATE_ORANGE:
            printf("ORANGE\n");
            break;
        case STATE_BLUE:
            printf("BLUE\n");
            break;
        case STATE_RED:
            printf("RED\n");
            break;
        case STATE_YELLOW:
            printf("YELLOW\n");
            break;
        case STATE_BLACK:
            printf("BLACK\n");
            break;
        case STATE_GREEN:
            printf("GREEN\n");
            break;
    }

    printf("\ttimeSinceInfection: %d\n", cell.timeSinceInfected);
    printf("} \n\n");

}

