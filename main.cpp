#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

vector<string> primaryInputs, primaryOutputs, internalsignals;
unordered_map<string, int> maxdelays;
unordered_map<string, vector<pair<string, int>>> out_to_in_signal_delay;
vector<pair<vector<string>, int>> signaltodelay;
unordered_map<string, int> maxdelays2;
unordered_map<string, int> requiredoutputdelays;

bool compareGates(pair<vector<string>, int> p1, pair<vector<string>, int> p2)
{
    return p1.second > p2.second;
}

int delay(string signal)
{
    int max_delay = 0;
    // base case
    if (find(primaryInputs.begin(), primaryInputs.end(), signal) != primaryInputs.end())
    {
        return 0;
    }
    else if (maxdelays.find(signal) != maxdelays.end())
    {
        return maxdelays[signal];
    }
    // recursice case
    vector<pair<string, int>> signaltodelay = out_to_in_signal_delay[signal];
    for (auto pair : signaltodelay)
    {
        max_delay = max(max_delay, pair.second + delay(pair.first));
    }
    maxdelays[signal] = max_delay;

    return max_delay;
}

int delay2(string s, unordered_map<string, vector<pair<string, int>>> u, unordered_map<string, int> od, unordered_map<string, int> maxdelays2)
{
    int max_delay2 = INT_MIN;
    // base case
    if (od.find(s) != od.end())
    {
        return od[s];
    }
    else if (maxdelays2.find(s) != maxdelays2.end())
    {
        return maxdelays2[s];
    }

    // recursice case

    vector<pair<string, int>> signaltodelay = u[s];
    for (auto pair : signaltodelay)
    {
        max_delay2 = max(max_delay2, pair.second + delay2(pair.first, u, od, maxdelays2));
    }
    maxdelays2[s] = -1 * max_delay2;

    return max_delay2;
}

void part1()
{
    for (int i = 0; i < signaltodelay.size(); i++)
    {
        int l = signaltodelay[i].first.size();
        vector<pair<string, int>> v1;
        for (int j = 0; j < signaltodelay[i].first.size() - 1; j++)
        {
            v1.push_back({signaltodelay[i].first[j], signaltodelay[i].second});
        }
        out_to_in_signal_delay[signaltodelay[i].first[l - 1]] = v1;
    }
    ofstream resultFile("output_delays.txt");

    if (resultFile.is_open())
    {
        // Write content to the file
        for (auto input_signal : primaryOutputs)
        {
            resultFile << input_signal << " ";
            resultFile << delay(input_signal) << endl;
        }
        // Close the file
        resultFile.close();
    }
}

void part2()
{
    unordered_map<string, vector<pair<string, int>>> in_out_delay;
    for (auto element : signaltodelay)
    {
        string output_signal = element.first[element.first.size() - 1];
        for (auto input_signal : element.first)
        {
            if (input_signal == output_signal)
            {
                continue;
            }
            if (in_out_delay.find(input_signal) == in_out_delay.end())
            {
                in_out_delay[input_signal] = {};
            }
            vector<pair<string, int>> vec = in_out_delay[input_signal];
            // vec.push_back({output_signal,})
            vec.push_back({output_signal, element.second});
            in_out_delay[input_signal] = vec;
        }
    }

    ofstream outputFile("inputs_delays.txt");

    if (outputFile.is_open())
    {
        // Write content to the file
        for (auto input_signal : primaryInputs)
        {
            outputFile << input_signal << " ";
            outputFile << -1 * delay2(input_signal, in_out_delay, requiredoutputdelays, maxdelays2) << endl;
        }
        // Close the file
        outputFile.close();
    }
}

int main()
{

    ifstream gateDelayFile("gate_delays.txt");
    int inv = 0, or2 = 0, nor2 = 0, and2 = 0, nand2 = 0;
    string tem;

    while (gateDelayFile >> tem)
    {
        if (tem == "NAND2")
        {
            gateDelayFile >> nand2;
        }
        else if (tem == "AND2")
        {
            gateDelayFile >> and2;
        }
        else if (tem == "NOR2")
        {
            gateDelayFile >> nor2;
        }
        else if (tem == "OR2")
        {
            gateDelayFile >> or2;
        }
        else if (tem == "INV")
        {
            gateDelayFile >> inv;
        }
    }
    gateDelayFile.close();

    ifstream cicuitsFile("circuit.txt");
    string temp;

    while (cicuitsFile >> temp)
    {
        if (temp[0] != temp[1] || temp[1] != '/')
        {
            if (temp == "PRIMARY_INPUTS")
            {
                string x;
                while (cicuitsFile >> x)
                {
                    if (x == "PRIMARY_OUTPUTS" || x == "INTERNAL_SIGNALS")
                    {
                        temp = x;
                        break;
                    }
                    primaryInputs.push_back(x);
                }
            }

            if (temp == "PRIMARY_OUTPUTS")
            {
                string x;
                while (cicuitsFile >> x)
                {
                    if (x == "INTERNAL_SIGNALS" || x == "PRIMARY_INPUTS")
                    {
                        temp = x;
                        break;
                    }
                    primaryOutputs.push_back(x);
                }
            }

            if (temp == "INTERNAL_SIGNALS")
            {
                string x;
                while (cicuitsFile >> x)
                {
                    if (x == "INV" || x == "AND2" || x == "NAND2" || x == "NOR2" || x == "OR2")
                    {
                        temp = x;
                        break;
                    }
                    internalsignals.push_back(x);
                }
            }

            vector<string> temp2;
            string x;

            if (temp == "INV")
            {
                string x, y;
                cicuitsFile >> x >> y;
                signaltodelay.push_back({{x, y}, inv});
            }
            else if (temp == "AND2")
            {
                string x, y, z;
                cicuitsFile >> x >> y >> z;
                signaltodelay.push_back({{x, y, z}, and2});
            }
            else if (temp == "OR2")
            {
                string x, y, z;
                cicuitsFile >> x >> y >> z;
                signaltodelay.push_back({{x, y, z}, or2});
            }
            else if (temp == "NOR2")
            {
                string x, y, z;
                cicuitsFile >> x >> y >> z;
                signaltodelay.push_back({{x, y, z}, nor2});
            }
            else if (temp == "NAND2")
            {
                string x, y, z;
                cicuitsFile >> x >> y >> z;
                signaltodelay.push_back({{x, y, z}, nand2});
            }
        }
    }
    cicuitsFile.close();

    sort(signaltodelay.begin(), signaltodelay.end(), compareGates);

    ifstream in("required_delays.txt");
    string t;

    while (in >> t)
    {
        int t2;
        in >> t2;
        requiredoutputdelays[t] = -1 * t2;
    }
    in.close();
    part1();
    part2();

    return 0;
}