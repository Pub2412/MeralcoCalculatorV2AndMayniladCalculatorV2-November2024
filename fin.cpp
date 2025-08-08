#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <string>
#include <cstdlib>
using namespace std;

// ---- Constants and Functions for Water Billing ----
const double CUBIC_METER_RATE = 42.46;
const double VAT_RATE = 0.12;
const double ENV_CHARGE_RATE = 0.20;

double calculateBasicCharge(double cubicMeters) {
    return cubicMeters * CUBIC_METER_RATE;
}

double calculateEnvironmentalCharge(double basicCharge, double fcda) {
    return (basicCharge + fcda) * ENV_CHARGE_RATE;
}

double calculateVAT(double totalBeforeVAT) {
    return totalBeforeVAT * VAT_RATE;
}

double calculateTotalAmountDue(double totalBeforeVAT, double vat, double previousUnpaidAmount) {
    return totalBeforeVAT + vat + previousUnpaidAmount;
}

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char date[11];
    sprintf(date, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(date);
}

string getOffsetDate(int offsetDays) {
    time_t now = time(0);
    now += offsetDays * 24 * 60 * 60;
    tm *ltm = localtime(&now);
    char date[11];
    sprintf(date, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(date);
}

double calculateMSC(int meterSize) {
    if (meterSize >= 13 && meterSize <= 19) return 1.50;
    else if (meterSize >= 20 && meterSize <= 24) return 2.00;
    else if (meterSize >= 25 && meterSize <= 39) return 3.00;
    else if (meterSize >= 40 && meterSize <= 49) return 4.00;
    else if (meterSize >= 50 && meterSize <= 74) return 6.00;
    else if (meterSize >= 75 && meterSize <= 99) return 10.00;
    else if (meterSize >= 100 && meterSize <= 149) return 20.00;
    else if (meterSize >= 150 && meterSize <= 199) return 35.00;
    else if (meterSize >= 200) return 50.00;
    return 1.50;
}

// ---- Constants and Functions for Electric Billing ----
const double RATE_PER_UNIT = 0.1543;

double computeGenerationCharge(double unitsConsumed) {
    return unitsConsumed * 0.55;
}

double computeTransmissionCharge(double unitsConsumed) {
    return unitsConsumed * 0.101;
}

double computeSystemLossCharge(double unitsConsumed, int hoursWithoutPower) {
    double lossCharge = unitsConsumed * RATE_PER_UNIT;
    double reduction = (hoursWithoutPower * 0.01) * lossCharge;
    return lossCharge - reduction;
}

double computeDistributionCharge(double unitsConsumed) {
    return unitsConsumed * 0.175;
}

double computeGovernmentTaxes(double totalAmount) {
    return totalAmount * 0.12;
}

double computeUniversalCharges(double totalAmount) {
    return totalAmount * 0.01;
}

double computeFitAllCharge(double unitsConsumed) {
    return unitsConsumed * RATE_PER_UNIT;
}

double computeAppliedCredits(double amount) {
    return amount * -0.05;
}

double computeInstallmentDue(double totalAmount) {
    return totalAmount * 0.1;
}

double computeSubsidies(double totalAmount, bool isSeniorCitizen, bool isLifeline) {
    if (isSeniorCitizen) return totalAmount * 0.20;
    if (isLifeline) return totalAmount * 0.10;
    return 0.0;
}

double computeTotalAmountDue(double generationCharge, double transmissionCharge,
                              double systemLossCharge, double distributionCharge,
                              double subsidies, double governmentTaxes,
                              double universalCharges, double fitAllCharge,
                              double appliedCredits, double otherCharges,
                              double installmentDue, double remainingBalance) {
    return generationCharge + transmissionCharge + systemLossCharge + distributionCharge +
           subsidies + governmentTaxes + universalCharges + fitAllCharge +
           appliedCredits + otherCharges + installmentDue + remainingBalance;
}

// ---- Main Program ----
int main() {
    int choice;
        cout << setw(50) << "===============================" << endl;
        cout << setw(50) << "|      BILLING CALCULATOR     |" << endl;
        cout << setw(50) << "===============================" << endl;
        cout << setw(44) << "\t      1. MAYNILAD Bill  Calculator\n";
        cout << setw(45) << "\t      2. MERALCO BILL Calculator\n";
        cout << setw(47) << "Input any number to Exit\n";
        cout << setw(50) << "===============================" << endl;
        cout << setw(40) << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        // Water Billing
        
        system("cls");
        
        double cubicMeters, previousUnpaidAmount;
        float meterSize;
        cout << "Enter water consumption in cubic meters: ";
        cin >> cubicMeters;
        cout << "Enter water meter size (mm): ";
        cin >> meterSize;
        cout << "Enter previous unpaid amount: ";
        cin >> previousUnpaidAmount;

		system("cls");
        double basicCharge = calculateBasicCharge(cubicMeters);
        double fcda = basicCharge * 0.0203;
        double environmentalCharge = calculateEnvironmentalCharge(basicCharge, fcda);
        double msc = calculateMSC(meterSize);
        double totalBeforeVAT = basicCharge + fcda + environmentalCharge + msc;
        double vat = calculateVAT(totalBeforeVAT);
        double totalAmountDue = calculateTotalAmountDue(totalBeforeVAT, vat, previousUnpaidAmount);
        double CC = basicCharge + fcda + environmentalCharge + msc + vat;
        
    // Billing period dates
    string billingEndDate = getCurrentDate();
    string billingStartDate = getOffsetDate(-30);
    string paymentDueDate = getOffsetDate(7);

 	// Output to console
	cout << fixed << setprecision(2);
	cout << "\n\t\t\t\t\t  B I L L I N G  S U M M A R Y\n";
	cout << "\n\t      BILLING PERIOD: " << billingStartDate << " to " << billingEndDate <<"\n";	
	cout << left << setw(40) << "\t      Current Charges" << right  << setw(30) << "\t " << CC << endl;
	cout << left << setw(40) << "\n\t\tBasic Charge"  << right << setw(25)  << "\t " << basicCharge << endl;
	cout << left << setw(40) << "\t\tFCDA"  << right << setw(25) << "\t " <<  fcda << endl;
	cout << left << setw(40) << "\t\tEnvironmental Charges (20% of Basic Charge and FCDA)" << right << setw(10) << "\t " << environmentalCharge << endl;
	cout << left << setw(40) << "\t\tMaintenance Service Charge (MSC)" << right << setw(25) << "\t " << msc << endl;
	cout << left << setw(40) << "\t\tTotal Current Charges before Taxes" << right << setw(25) << "\t " << totalBeforeVAT << endl;
	cout << left << setw(40) << "\t\tValue Added Tax (VAT)" << right << setw(25) << "\t " << vat << endl;
	cout << left << setw(40) << "\n\n\t\tPrevious Unpaid Amount (Please pay immediately)" << right << setw(17) << "\t " << previousUnpaidAmount << endl;
	cout << setw(50) << "               ----------------------------------------------------------------------------"<< endl;
	cout << left << setw(40) << "\t\tTOTAL AMOUNT DUE"  << right << setw(25) << "\tPHP " << totalAmountDue << endl;
	cout << left << setw(40) << "\t\tPAYMENT DUE DATE" << right << setw(36) << paymentDueDate << "\n";
	cout << setw(50) << "               ----------------------------------------------------------------------------"<< endl;
	cout << "\t\tPlease examine your bill carefully. If no complaint is made within 60 days \n"; 
    cout << "\t\tof the receipt, the bill is considered true and correct. \n";
    
	// Output to Word-compatible text file
	ofstream outFile("MaynilandBillingStatement.txt");
	outFile << fixed << setprecision(2);
	outFile << "\n\t\t\t\t\t  B I L L I N G  S U M M A R Y\n";
	outFile << "\n\t      BILLING PERIOD: " << billingStartDate << " to " << billingEndDate <<"\n";	
	outFile << left << setw(40) << "\t      Current Charges" << right  << setw(30) << "\t " << CC << endl;
	outFile << left << setw(40) << "\n\t\tBasic Charge"  << right << setw(25)  << "\t " << basicCharge << endl;
	outFile << left << setw(40) << "\t\tFCDA"  << right << setw(25) << "\t " <<  fcda << endl;
	outFile << left << setw(40) << "\t\tEnvironmental Charges (20% of Basic Charge and FCDA)" << right << setw(10) << "\t " << environmentalCharge << endl;
	outFile << left << setw(40) << "\t\tMaintenance Service Charge (MSC)" << right << setw(25) << "\t " << msc << endl;
	outFile << left << setw(40) << "\t\tTotal Current Charges before Taxes" << right << setw(25) << "\t " << totalBeforeVAT << endl;
	outFile << left << setw(40) << "\t\tValue Added Tax (VAT)" << right << setw(25) << "\t " << vat << endl;
	outFile << left << setw(40) << "\n\n\t\tPrevious Unpaid Amount (Please pay immediately)" << right << setw(17) << "\t " << previousUnpaidAmount << endl;
	outFile << setw(50) << "               ----------------------------------------------------------------------------"<< endl;
	outFile << left << setw(40) << "\t\tTOTAL AMOUNT DUE"  << right << setw(25) << "\tPHP " << totalAmountDue << endl;
	outFile << left << setw(40) << "\t\tPAYMENT DUE DATE" << right << setw(36) << paymentDueDate << "\n";
	outFile << setw(50) << "               ----------------------------------------------------------------------------"<< endl;
	outFile << "\t\tPlease examine your bill carefully. If no complaint is made within 60 days \n"; 
    outFile << "\t\tof the receipt, the bill is considered true and correct. \n";
	outFile.close();
	cout << "\n\n\t\tBilling statement saved to MaynilandBillingStatement.txt\n";

    } else if (choice == 2) {
        // Electric Billing
        
        system("cls");
        
        double previousReading, currentReading, unitsConsumed, remainingBalance, otherCharges;
        bool isSeniorCitizen, isLifeline;
        int hoursWithoutPower;
		
        cout << "Enter previous meter reading (in kWh): ";
        cin >> previousReading;
        cout << "Enter current meter reading (in kWh): ";
        cin >> currentReading;
        unitsConsumed = currentReading - previousReading;
        cout << "Enter remaining balance from the previous bill: ";
        cin >> remainingBalance;
        cout << "Are you a senior citizen (1 for yes, 0 for no)? ";
        cin >> isSeniorCitizen;
        cout << "Are you enrolled in the lifeline program (1 for yes, 0 for no)? ";
        cin >> isLifeline;
        cout << "Enter the number of hours without power: ";
        cin >> hoursWithoutPower;
    	cout << "Enter Other Charges (if any): Php ";
    	cin >> otherCharges;

        double generationCharge = computeGenerationCharge(unitsConsumed);
        double transmissionCharge = computeTransmissionCharge(unitsConsumed);
        double systemLossCharge = computeSystemLossCharge(unitsConsumed, hoursWithoutPower);
        double distributionCharge = computeDistributionCharge(unitsConsumed);
        double subtotalBeforeSubsidy = generationCharge + transmissionCharge + systemLossCharge + distributionCharge;
        double subsidies = computeSubsidies(subtotalBeforeSubsidy, isSeniorCitizen, isLifeline);
        double governmentTaxes = computeGovernmentTaxes(subtotalBeforeSubsidy);
        double universalCharges = computeUniversalCharges(subtotalBeforeSubsidy);
        double fitAllCharge = computeFitAllCharge(unitsConsumed);
        double appliedCredits = computeAppliedCredits(subtotalBeforeSubsidy);
        double installmentDue = computeInstallmentDue(subtotalBeforeSubsidy);
        double totalAmountDue = computeTotalAmountDue(generationCharge, transmissionCharge, systemLossCharge, distributionCharge,
                                                      subsidies, governmentTaxes, universalCharges, fitAllCharge,
                                                      appliedCredits, 0, installmentDue, remainingBalance);

   
       system("cls");    
	  // Output the same information to the console
    cout << fixed << setprecision(2);  // To format the output to 2 decimal places

    // Center the header
    cout << "\n";
    cout << setw(60) << "----------------------- Meralco Bill -------------------" << endl;

    // Output the bill details to console
    cout << right << setw(40) << "Remaining Balance from Previous Bill:" << setw(20) << remainingBalance << endl;
    cout << right << setw(40) << "Charges for this Billing Period:" << setw(20) << (generationCharge + transmissionCharge + systemLossCharge + governmentTaxes + universalCharges + fitAllCharge + otherCharges)-appliedCredits << endl;
    cout << right << setw(40) << "Generation Charge:" << setw(20) << generationCharge << endl;
    cout << right << setw(40) << "Transmission Charge:" << setw(20) << transmissionCharge << endl;
    cout << right << setw(40) << "System Loss Charge:" << setw(20) << systemLossCharge << endl;
    cout << right << setw(40) << "Distribution (Meralco) Charge:" << setw(20) << distributionCharge << endl;
    cout << right << setw(40) << "Subsidies:" << setw(20) << subsidies << endl;
    cout << right << setw(40) << "Government Taxes:" << setw(20) << governmentTaxes << endl;
    cout << right << setw(40) << "Universal Charges:" << setw(20) << universalCharges << endl;
    cout << right << setw(40) << "FIT-All (Renewable) Charge:" << setw(20) << fitAllCharge << endl;
    cout << right << setw(40) << "Applied Credits:" << setw(20) << appliedCredits << endl;
    cout << right << setw(40) << "Other Charges:" << setw(20) << otherCharges << endl;
    cout << right << setw(40) << "Installment Due:" << setw(20) << installmentDue << endl;
    cout << "\n";
    cout << "\n";
    cout << right << setw(35) << "Total Amount Due:" << setw(20) << "PHP " << totalAmountDue << endl; 
   
   // Output the details to a text file
    ofstream outFile("MeralcoBillingStatement.txt");  // Create and open the output file

    if (outFile.is_open()) {
        outFile << fixed << setprecision(2);  // To format the output to 2 decimal places

        // Center the header
        outFile << "\n";
    	outFile << setw(60) << "----------------------- Meralco Bill -------------------" << endl;

        // Output the bill details to file
        outFile << right << setw(40) << "Remaining Balance from Previous Bill:" << setw(20) << remainingBalance << endl;
        outFile << right << setw(40) << "Charges for this Billing Period:" << setw(20) << (generationCharge + transmissionCharge + systemLossCharge + governmentTaxes + universalCharges + fitAllCharge + otherCharges)-appliedCredits << endl;
        outFile << right << setw(40) << "Generation Charge:" << setw(20) << generationCharge << endl;
        outFile << right << setw(40) << "Transmission Charge:" << setw(20) << transmissionCharge << endl;
        outFile << right << setw(40) << "System Loss Charge:" << setw(20) << systemLossCharge << endl;
        outFile << right << setw(40) << "Distribution (Meralco) Charge:" << setw(20) << distributionCharge << endl;
        outFile << right << setw(40) << "Subsidies:" << setw(20) << subsidies << endl;
        outFile << right << setw(40) << "Government Taxes:" << setw(20) << governmentTaxes << endl;
        outFile << right << setw(40) << "Universal Charges:" << setw(20) << universalCharges << endl;
        outFile << right << setw(40) << "FIT-All (Renewable) Charge:" << setw(20) << fitAllCharge << endl;
        outFile << right << setw(40) << "Applied Credits:" << setw(20) << appliedCredits << endl;
        outFile << right << setw(40) << "Other Charges:" << setw(20) << otherCharges << endl;
        outFile << right << setw(40) << "Installment Due:" << setw(20) << installmentDue << endl;

    	outFile << "\n";
    	outFile << "\n";
    	outFile << right << setw(35) << "Total Amount Due:" << setw(20) << "PHP " << totalAmountDue << endl; 

        outFile.close();  // Close the file
        cout << "\nBill has been saved to 'MeralcoBillingStatement.txt'" << endl;
    } else {
        cout << "Error opening file for writing." << endl;
    }



    } else {
        cout << "Goodbye!.\n";
    }

    return 0;
}

