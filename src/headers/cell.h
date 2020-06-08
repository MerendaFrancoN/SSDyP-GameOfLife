#ifndef SSDYP_GAMEOFLIFE_CELL_H
#define SSDYP_GAMEOFLIFE_CELL_H

//Max Numbers of enums
#define RISK_DISEASES_NUMBER 5
#define RISK_PROFESSIONS_NUMBER 4
#define PREVENTIVE_VACCINES_NUMBER 4


/* State - Possible states of cells
 * WHITE - Free Cell
 * BLUE - Person susceptible to the illness
 * ORANGE - Ill without contagious
 * RED - Ill with contagious
 * YELLOW - Ill but isolated
 * GREEN - Recovered
 * BLACK - Dead
 */
enum state{
    STATE_WHITE,
    STATE_BLUE,
    STATE_ORANGE,
    STATE_RED,
    STATE_YELLOW,
    STATE_GREEN,
    STATE_BLACK
};

/* Possible Ages -
 * CHILD - 18
 * ADULT - 18 - 60
 * OLD - 60+
 * */
enum age{
    AGE_CHILD,
    AGE_ADULT,
    AGE_OLD
};

/* Risk Diseases
 * NO - NOT HAVE
 * DIABETES
 * HYPERTENSION
 * RESPIRATORY_DISEASE,
 * IMMUNODEPRESSED
 *
 * Can add more to this list.
 */
enum risk_diseases{
    RISK_DISEASE_NO = -1,
    RISK_DISEASE_DIABETES,
    RISK_DISEASE_HYPERTENSION,
    RISK_DISEASE_RESPIRATORY_DISEASE,
    RISK_DISEASE_IMMUNODEPRESSED
};

/* Proffessions
 * ENFERMERY
 * DENTIST
 * PUBLIC_CHAUFFEUR
 *
 * Can add more to this list.
*/
enum risk_professions{
    RISK_PROF_NO = -1,
    RISK_PROF_ENFERMERY,
    RISK_PROF_DENTIST,
    RISK_PROF_PUBLIC_CHAUFFEUR,
};

// Preventive Vaccines
enum preventive_vaccines{
    VACC_NOT_VACCINATED = -1,
    VACC_FLU,
    VACC_PNEUMONIA,
    VACC_MENINGITIS
};

//Biological Sex
enum biological_sex{
    SEX_MAN,
    SEX_WOMAN
};

/*
 * Cell with all the information that it is needed
 * to perform calculations, give info and generate
 * the next state.
 *
*/
typedef struct cell_{
    char state;
    char age;
    char risk_disease;
    char risk_professions;
    char preventive_vaccines;
    char biological_sex;
    unsigned long int timeSinceInfected;
} cell_type;

/* --UTILS Functions-- */

//Create a random cell with some configuration
cell_type createRandomCell(double childRate, double adultRate, double oldRate, double infectionRate);

//Function to print a cell
void printCell(cell_type cell);

// Function to get the age from an integer
char intToAge(unsigned short int intAge);

// Function to determine susceptibility
double susceptibility(char age, char risk_disease);

// Function that returns the probability to go from BLUE to ORANGE state.
double probability_P(double cellsContagious, double covidPower, double susceptibility, short unsigned int stepsByDay);

/* Returns the illness death rate in function of the age and if
 * the person is vaccinated or not. */
double illness_death_rate(char age, char preventiveVaccines);

// Function that from a cell, generates the state for the next cell
char state_machine(char currentState, unsigned long int timeSinceInfected, char age, char risk_disease, char preventiveVaccines, double cellsContagious);

#endif //SSDYP_GAMEOFLIFE_CELL_H
