#include "cell.h"
/* --UTILS Functions-- */

// Function to determine susceptibility
double susceptibility(char age, char risk_disease){
    double susceptibility = (risk_disease == RISK_DISEASE_NO) ? 0.15 : 0.0;

    if(age == AGE_CHILD)
        return susceptibility + 0.3;
    if(age == AGE_ADULT)
        return susceptibility + 0.5;
    if(age == AGE_OLD)
        return susceptibility + 0.9;

    return 0.0;
}

// Function that returns the probability to go from BLUE to ORANGE state.
double probability_P(double cellsContagious, double covidPower, double susceptibility, short unsigned int stepsByDay){
    return (cellsContagious * covidPower + susceptibility) / (double) (7.0 * (double)stepsByDay);
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

    return 0.0;
}

// Function that from a cell, generates the state for the next cell
cell_type next_state(cell_type currentState, double cellsContagious, double covidPower) {

    //Check if a null state, return as it was.
    if (currentState.state == STATE_WHITE )
        return currentState;

    /*Update Time */
    //Get the current Time
    unsigned long currentTime = currentState.timeSinceInfected;

    if(currentState.state >= STATE_ORANGE && currentState.timeSinceInfected <= 14)
        currentState.timeSinceInfected++;

    /* Update States */
    switch (currentState.state) {
        case STATE_BLUE:{
            //If there are no cell contagious near and the P probability
            if( cellsContagious != 0.0 && randomDoubleGenerator() > probability_P(cellsContagious, covidPower, susceptibility( currentState.age, currentState.risk_disease), 1) ) {
                //Set new state
                currentState.state = STATE_ORANGE;
                return currentState;
            }
            return currentState;
        }
        case STATE_ORANGE:{
            //If t = 4
            if (currentTime == 4) {
                //Set new state
                currentState.state = STATE_RED;
                return currentState;
            }
            return currentState;
        }
        case STATE_RED:
        case STATE_YELLOW:{
            /*Case of decide if Isolate - Decide to go to STATE_YELLOW*/
            if( currentTime == 7 ){
                //Probability given by problem ( 0.9 )
                if( randomDoubleGenerator() <= 0.9){ //With 90% of probability will go to Yellow state
                    //Set the new state
                    currentState.state = STATE_YELLOW;
                    return currentState;
                }
            }

            //Check if we are at the end of illness
            if( currentTime == 14){

                /*Case of Recovered or Death */

                if(randomDoubleGenerator() > illness_death_rate( currentState.age, currentState.preventive_vaccines)){
                    //Set the new state
                    currentState.state = STATE_BLACK;
                    return currentState;
                }else{
                    //Set the new state
                    currentState.state = STATE_GREEN;
                    return currentState;
                }
            }

            return currentState;
        } //End case

        default: return currentState;

    }

}

cell_type createNullCell(){
    cell_type cellToReturn;
    cellToReturn.age = AGE_NO_VALUE;
    cellToReturn.state = STATE_WHITE;
    cellToReturn.risk_professions = RISK_PROF_NO;
    cellToReturn.risk_disease = RISK_DISEASE_NO;
    cellToReturn.timeSinceInfected = 0;
    cellToReturn.biological_sex = SEX_NO_VALUE;
    cellToReturn.preventive_vaccines = VACC_NOT_VACCINATED;
    return cellToReturn;
}

cell_type createInvalidCell(){
    cell_type cellToReturn;
    cellToReturn.age = AGE_NO_VALUE;
    cellToReturn.state = STATE_INVALID;
    cellToReturn.risk_professions = RISK_PROF_NO;
    cellToReturn.risk_disease = RISK_DISEASE_NO;
    cellToReturn.timeSinceInfected = 0;
    cellToReturn.biological_sex = SEX_NO_VALUE;
    cellToReturn.preventive_vaccines = VACC_NOT_VACCINATED;
    return cellToReturn;
}

//Function to create a cell with some configuration
cell_type createRandomCell(double childRate, double adultRate, double oldRate, double infectionRate){

    //Declare the new cell
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
    cellToReturn.risk_disease = (char) randomEnumIntGenerator(-1, RISK_DISEASES_NUMBER);

    //Generate Random Risk Professions
    cellToReturn.risk_professions = ( cellToReturn.age == AGE_CHILD ) ?
                                    RISK_PROF_NO :
                                    randomEnumIntGenerator(-1, RISK_PROFESSIONS_NUMBER);

    //Generate Random Preventive Vaccines
    cellToReturn.preventive_vaccines = (char) randomEnumIntGenerator(-1, PREVENTIVE_VACCINES_NUMBER);
    //Generate Random Sex
    cellToReturn.biological_sex = (char) randomEnumIntGenerator(0, 2);

    //Generate Random State
    randomValue = randomDoubleGenerator();
    cellToReturn.state = (randomValue > infectionRate) ? STATE_BLUE : STATE_RED;
    cellToReturn.timeSinceInfected =(randomValue > infectionRate) ? 0 : 5;

    //Return cell
    return cellToReturn;
}

//Function to print a Cell
void printCell(cell_type cell){
    printf("{\n");

    //Print Age
    printf("\tage: ");
    switch (cell.age) {
        case AGE_NO_VALUE:
            printf("NO VALUE\n");
            break;
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
        case STATE_INVALID:
            printf("INVALID\n");
            break;
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

    printf("\ttimeSinceInfection: %lu\n", cell.timeSinceInfected);
    printf("} \n\n");

}

