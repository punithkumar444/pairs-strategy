#include <bits/stdc++.h>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;
double CDF(double X, double Mean, double Variance)
{
    X = (X - Mean) / sqrt(Variance);
    return (1.0 + erf(X / sqrt(2.0))) / 2.0;
}
void replaceCommasWithSemicolons(string &line)
{
    bool inQuotes = false;
    for (char &c : line)
    {
        if (c == '"')
        {
            inQuotes = !inQuotes;
        }
        else if (inQuotes && c == ',')
        {
            c = ';';
        }
    }
}
void CleanTheInputFile(string s, string num)
{
    ifstream inputFile(s);
    ofstream outputFile(num);
    string line;
    while (getline(inputFile, line))
    {
        replaceCommasWithSemicolons(line);
        outputFile << line << '\n';
    }
    inputFile.close();
    outputFile.close();
}
vector<double> Read_from_CSV(const string &filename)
{
    vector<string> Previous_closing_price;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening the file!" << endl;
    }
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        stringstream ss(line);
        string cell;
        for (int i = 0; i <= 5; ++i)
        {
            if (!getline(ss, cell, ','))
            {
                Previous_closing_price;
            }
        }
        Previous_closing_price.push_back(cell);
    }
    file.close();
    vector<double> Previous_closing_price_int;
    for (const string &value : Previous_closing_price)
    {
        string result;
        for (char c : value)
        {
            if (isdigit(c) || c == '.')
            {
                result += c;
            }
        }
        double Price = stof(result);
        Previous_closing_price_int.push_back(Price);
    }
    return Previous_closing_price_int;
}
vector<double> Percent_daily_returns(vector<double> Closing_prices)
{
    vector<double> Percent_daily_returns;
    for (int i = 0; i < Closing_prices.size() - 1; i++)
    {
        double f = (((Closing_prices[i + 1] / Closing_prices[i]) - 1) * 100);
        Percent_daily_returns.push_back(f);
    }
    return Percent_daily_returns;
}
vector<double> Percent_daily_returns_future(vector<double> Stock_future_cp, int last)
{
    vector<double> Stock_1_future_daily_returns;
    Stock_1_future_daily_returns.push_back((Stock_future_cp[0] / last) - 1);
    for (int i = 0; i < Stock_1_future_daily_returns.size() - 1; i++)
    {
        double f = (((Stock_1_future_daily_returns[i + 1] / Stock_1_future_daily_returns[i]) - 1) * 100);
        Stock_1_future_daily_returns.push_back(f);
    }
    return Stock_1_future_daily_returns;
}
double Mean(vector<double> Vector_to_calculte_mean)
{
    double Mean = 0;
    for (int i = 0; i < Vector_to_calculte_mean.size(); i++)
    {
        Mean = Mean + Vector_to_calculte_mean[i];
    }
    Mean = Mean / Vector_to_calculte_mean.size();
    return Mean;
}
double Variance(vector<double> Vector_to_calculate_variance)
{
    double Variance = 0;
    double Mean_of_given_vector = Mean(Vector_to_calculate_variance);
    for (int i = 0; i < Vector_to_calculate_variance.size(); i++)
    {
        Variance = Variance + ((Vector_to_calculate_variance[i] - Mean_of_given_vector) * (Vector_to_calculate_variance[i] - Mean_of_given_vector));
    }
    Variance = Variance / (Vector_to_calculate_variance.size() - 1);
    return Variance;
}
double Expectation_value(vector<double> DailyReturns)
{
    double Expectation;
    double sum = 0;
    for (int i = 0; i < DailyReturns.size(); i++)
    {
        sum += DailyReturns[i];
    }
    Expectation = sum / DailyReturns.size();
    return Expectation;
}
vector<double> Vector_product(vector<double> X, vector<double> Y)
{
    vector<double> X_and_Y;
    for (int i = 0; i < X.size(); i++)
    {
        double f;
        f = X[i] * Y[i];
        X_and_Y.push_back(f);
    }
    return X_and_Y;
}
double Covariance(vector<double> X, vector<double> Y)
{
    double Covariance;
    // Cov(X,Y) = E(XY) - E(X)*E(Y)
    vector<double> X_and_Y = Vector_product(X, Y);
    Covariance = Expectation_value(X_and_Y) - (Expectation_value(X) * (Expectation_value(Y)));
    return Covariance;
}
vector<double> PDF(vector<double> Ratio_of_values)
{
    double Mean1 = Mean(Ratio_of_values);
    double Variance1 = Variance(Ratio_of_values);
    vector<double> Probability_density_function;
    double ans = 0;
    for (int i = 0; i < Ratio_of_values.size(); i++)
    {
        ans = (1 / sqrt(Variance1 * 2 * 3.14)) * exp(-1 * ((Ratio_of_values[i] - Mean1) * (Ratio_of_values[i] - Mean1)) / (2 * Variance1));
        Probability_density_function.push_back(ans);
    }
    return Probability_density_function;
}
int main()
{
    string Ticker_symbol_1, Ticker_symbol_2;
    cout << "Please enter the Ticker symbols of Stock 1 and Stock 2 respectively" << endl;
    cin >> Ticker_symbol_1 >> Ticker_symbol_2;
    string file1 = Ticker_symbol_1 + ".csv";
    string file2 = Ticker_symbol_2 + ".csv";
    string file3 = Ticker_symbol_1 + "-future.csv";
    string file4 = Ticker_symbol_2 + "-future.csv";
    CleanTheInputFile(file1, file1 + "_clean");
    CleanTheInputFile(file2, file2 + "_clean");
    CleanTheInputFile(file3, file3 + "_clean");
    CleanTheInputFile(file4, file4 + "_clean");
    vector<int> type;
    vector<double> Stock_1_previous_closing_price = Read_from_CSV(file1 + "_clean");
    vector<double> Stock_2_previous_closing_price = Read_from_CSV(file2 + "_clean");
    vector<double> Stock_1_future_cp = Read_from_CSV(file3 + "_clean");
    vector<double> Stock_2_future_cp = Read_from_CSV(file4 + "_clean");

    // Calculating the percentage of daily returns.
    vector<double> Stock_1_percent_daily_returns = Percent_daily_returns(Stock_1_previous_closing_price);
    vector<double> Stock_2_percent_daily_returns = Percent_daily_returns(Stock_2_previous_closing_price);

    // We write a function to calculate the covariance using the formula thought in the class
    // Cov(X,Y) = E(XY) - E(X)*E(Y)
    // Now we can calculate the Correlation between the two stocks using the formula thought in the class
    // Correlation = Cov(X,Y)/sqrt(Cov(X,X)*Cov(Y,Y))

    double Correlation = Covariance(Stock_1_percent_daily_returns, Stock_2_percent_daily_returns) / sqrt(Covariance(Stock_1_percent_daily_returns, Stock_1_percent_daily_returns) * Covariance(Stock_2_percent_daily_returns, Stock_2_percent_daily_returns));
    if (Correlation > 0.6)
    {
        cout << "The Correlation between the two stocks is " << Correlation << " which is greater than 0.60." << endl
             << "Hence we can go ahead and use the strategy." << endl;
        // Clearly the two stocks are Correlated to each other
        // We now calculate the difference of the scaled Previous closing prices of the stocks.
        vector<double> Diff_of_scaled_closing_price;
        double f1;
        for (int i = 0; i < Stock_1_previous_closing_price.size(); i++)
        {
            f1 = -(Stock_1_previous_closing_price[i]) * Mean(Stock_2_previous_closing_price) / Mean(Stock_1_previous_closing_price) + (Stock_2_previous_closing_price[i]);
            if (isfinite(f1))
            {
                Diff_of_scaled_closing_price.push_back(f1);
            }
        }

        // Calculating the mean of difference of the scaled Daily closing price.
        double Mean_of_daily_log_of_ratio_of_returns = Mean(Diff_of_scaled_closing_price);

        // Calculating the variance of the difference of the scaled Daily closing price.
        double Variance_of_log_of_daily_ratio_of_returns = Variance(Diff_of_scaled_closing_price);
        string s1, s2;

        cout << "Do you want the vector containing the difference of the scaled Daily closing price. ??...(y or n) : ";
        cin >> s1;
        if (s1 == "y")
        {
            cout << "X = ";
            cout << "[";
            for (double i : Diff_of_scaled_closing_price)
            {
                cout << i << ",";
            }
            cout << "];" << endl
                 << endl;
        }

        // Code for generating trading signals.

        for (int i = 0; i < min(Stock_1_future_cp.size(), Stock_2_future_cp.size()); i++)
        {
            float curr_scaled_diff_of_cp;
            double f11 = -(Stock_1_future_cp[i]) * (Mean(Stock_2_previous_closing_price)) / (Mean(Stock_1_previous_closing_price)) + Stock_2_future_cp[i];
            if (isfinite(f11))
            {
                curr_scaled_diff_of_cp = f11;
            }
            else
            {
                continue;
            }
            if (4 < curr_scaled_diff_of_cp)
            {
                int Quantity_of_Stock_1, Quantity_of_Stock_2;
                Quantity_of_Stock_1 = 2500;
                Quantity_of_Stock_2 = 2500 * Stock_1_future_cp[i] / Stock_2_future_cp[i];
                string s;
                cout << "The current scaled difference is more than the most probable scaled difference range." << endl
                     << "Hence the probability of the current scaled difference to reduce is 0.6" << endl;
                cout << "Do you want to take the trade....(y or n) -> ";
                cin >> s;
                cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                int number_of_days = 0;
                if (s == "y")
                {
                    cout << endl
                         << endl
                         << "Buy " << Ticker_symbol_1 << " at " << Stock_1_future_cp[i] << " and sell " << Ticker_symbol_2 << " at " << Stock_2_future_cp[i] << endl;
                    cout << "According to the Mean ratio of closing prices." << endl
                         << "Buy Quantity = " << Quantity_of_Stock_2 << " Sell Quantity = " << Quantity_of_Stock_1 << endl;
                    double Buy_Price_of_Stock_1 = Stock_1_future_cp[i];
                    double Sell_Price_of_Stock_2 = Stock_2_future_cp[i];
                    cout << "TRADE IS SUCCESFULLY EXECUTED." << endl
                         << endl
                         << endl;
                    i++;
                    while (i < Stock_1_future_cp.size() - 1)
                    {
                        double f12 = -(Stock_1_future_cp[i]) * (Mean(Stock_2_previous_closing_price)) / (Mean(Stock_1_previous_closing_price)) + Stock_2_future_cp[i];
                        curr_scaled_diff_of_cp = f12;
                        number_of_days++;
                        // cout << "Keep the trade open" << endl;
                        if (curr_scaled_diff_of_cp <= 0 && curr_scaled_diff_of_cp >= -4)
                        {
                            cout << endl
                                 << endl
                                 << endl
                                 << "Sell the stock 1 at " << Stock_1_future_cp[i] << " and buy the stock 2 at " << Stock_2_future_cp[i] << endl
                                 << endl;
                            cout << "TRADE CLOSED SUCCESFULLY" << endl
                                 << endl;
                            cout << "Number of days the trade was open : " << number_of_days << endl;
                            cout << "The Profit from the trade is : " << (-Buy_Price_of_Stock_1 + Stock_1_future_cp[i]) * Quantity_of_Stock_1 + (Sell_Price_of_Stock_2 - Stock_2_future_cp[i]) * Quantity_of_Stock_2 << endl;
                            cout << "Average return daily : " << ((-Buy_Price_of_Stock_1 + Stock_1_future_cp[i]) * Quantity_of_Stock_1 + (Sell_Price_of_Stock_2 - Stock_2_future_cp[i]) * Quantity_of_Stock_2) / number_of_days << endl
                                 << endl
                                 << endl;
                            cout<< "The total capital employed during this trade was -> "<<Buy_Price_of_Stock_1*Quantity_of_Stock_1 + Sell_Price_of_Stock_2*Quantity_of_Stock_2<<endl;
                            float percent_return_on_cap_emp = (((-Buy_Price_of_Stock_1 + Stock_1_future_cp[i]) * Quantity_of_Stock_1 + (Sell_Price_of_Stock_2 - Stock_2_future_cp[i]) * Quantity_of_Stock_2)/(Buy_Price_of_Stock_1*Quantity_of_Stock_1 + Sell_Price_of_Stock_2*Quantity_of_Stock_2))*100;
                            cout<< "The percentage return on the capital employed -> "<<percent_return_on_cap_emp<<" % "<<endl;
                            cout<< "The annual return of this strategy is -> "<<(percent_return_on_cap_emp/number_of_days)*365<<" % "<<endl;
                            cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                            break;
                        }
                        i++;
                    }
                }
            }
            if (curr_scaled_diff_of_cp < -6)
            {
                int Quantity_of_Stock_1, Quantity_of_Stock_2;
                Quantity_of_Stock_1 = 2500;
                Quantity_of_Stock_2 = 2500 * Stock_1_future_cp[i] / Stock_2_future_cp[i];
                string s;
                cout << "The current scaled difference is less than the most probable scaled difference range." << endl
                     << "Hence the probability of the current scaled difference to increase is 0.6" << endl;
                cout << "Do you want to take the trade....(y or n) -> ";
                cin >> s;
                cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                int number_of_days = 0;
                if (s == "y")
                {
                    cout << endl
                         << endl
                         << "Sell " << Ticker_symbol_1 << " at " << Stock_1_future_cp[i] << " and buy " << Ticker_symbol_2 << " at " << Stock_2_future_cp[i] << endl;
                    cout << "According to the Mean ratio of closing prices." << endl
                         << "Sell Quantity = " << Quantity_of_Stock_1 << "Buy Quantity = " << Quantity_of_Stock_2 << endl;
                    double Sell_Price_of_Stock_1 = Stock_1_future_cp[i];
                    double Buy_Price_of_Stock_2 = Stock_2_future_cp[i];
                    cout << "TRADE IS SUCCESFULLY EXECUTED." << endl
                         << endl
                         << endl;
                    i++;
                    while (i < Stock_1_future_cp.size() - 1)
                    {
                        double f12 = -(Stock_1_future_cp[i]) * (Mean(Stock_2_previous_closing_price)) / (Mean(Stock_1_previous_closing_price)) + Stock_2_future_cp[i];
                        curr_scaled_diff_of_cp = f12;
                        number_of_days++;
                        // cout << "Keep the trade open" << endl;
                        if (curr_scaled_diff_of_cp >= -4 && curr_scaled_diff_of_cp <= 0)
                        {
                            cout << endl
                                 << endl
                                 << endl
                                 << "Buy the stock 1 at " << Stock_1_future_cp[i] << " and Sell the stock 2 at " << Stock_2_future_cp[i] << endl
                                 << endl;
                            cout << "TRADE CLOSED SUCCESFULLY" << endl
                                 << endl;
                            cout << "Number of days the trade was open : " << number_of_days << endl;
                            cout << "The Profit from the trade is : " << (Sell_Price_of_Stock_1 - Stock_1_future_cp[i]) * Quantity_of_Stock_1 + (-Buy_Price_of_Stock_2 + Stock_2_future_cp[i]) * Quantity_of_Stock_2 << endl;
                            cout << "Average return daily : " << ((Sell_Price_of_Stock_1 - Stock_1_future_cp[i]) * Quantity_of_Stock_1 + (-Buy_Price_of_Stock_2 + Stock_2_future_cp[i]) * Quantity_of_Stock_2) / number_of_days << endl
                                 << endl
                                 << endl;
                            cout<< "The total capital employed during this trade was -> "<<Sell_Price_of_Stock_1*Quantity_of_Stock_1 + Buy_Price_of_Stock_2*Quantity_of_Stock_2<<endl;
                            cout<< "The percentage return on the capital employed -> "<<((Sell_Price_of_Stock_1 - Stock_1_future_cp[i]) * Quantity_of_Stock_1 + (-Buy_Price_of_Stock_2 + Stock_2_future_cp[i]) * Quantity_of_Stock_2)/(Sell_Price_of_Stock_1*Quantity_of_Stock_1 + Buy_Price_of_Stock_2*Quantity_of_Stock_2)<<endl;
                            cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                            break;
                        }
                        i++;
                    }
                }
            }
        }
    }
    else
    {
        cout << "The Correlation between the two stocks is " << Correlation << " which is less than or equal to 0.65." << endl
             << "Hence we can not use the strategy." << endl
             << "We need to find a new pair." << endl;
    }
    return 0;
}