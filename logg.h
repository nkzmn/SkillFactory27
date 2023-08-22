#pragma once
#include <fstream>
#include <string>
#include <shared_mutex>


static int counterLogString = 0;
class Logg {
public:
    Logg(const std::string& filename) : file(filename) {}

    ~Logg() {
        file.close();
    }

    //std::fstream& 
    void writeLog(std::ostream& outputStream, std::fstream& file) {
        std::unique_lock lock(mutex);
        if (!file)
            std::cout << "file not open!";
        std::streambuf* oldCoutStreamBuf = outputStream.rdbuf(); // ���������� �������� ������ ������
        file << ++counterLogString << ". " << oldCoutStreamBuf << std::endl;
    }

    void writeLog(const char* outputStr, std::fstream& file) {
        std::unique_lock lock(mutex);
        if (!file) {
            std::cout << "file not open!";
            return;
        }
        std::streambuf* oldCoutStreamBuf = std::cout.rdbuf(); // ��������� �������� ������ ������

        file << ++counterLogString << ". " << "Message received: " << outputStr << std::endl;
    }

    void LogString() {
        std::unique_lock lock(mutex);
        file.close();
        file.open("log.txt", std::ios::in);
        file.seekg(0);
        if (!file) { // �������� �� �������� �������� �����
            std::cout << "Error opening file." << std::endl;
        }

        std::vector<std::string> lines; // ������ ��� �������� �����

        std::string line{ 0 };
        while (std::getline(file, line)) { // ��������� ������ �� �����
            lines.push_back(line);
        }

        if (lines.empty()) { // �������� ������� ����� � �����
            //std::cout << "File is empty." << std::endl;
            return;
        }

        std::string lastLine = lines.back(); // �������� ��������� ������

        size_t dotPosition = lastLine.find_first_of('.'); // ������� ������� ��������� �����

        if (dotPosition == std::string::npos) { // �������� ������� ����� � ������
            std::cout << "Dot not found in the last line." << std::endl;
        }

        std::string charactersBeforeDot = lastLine.substr(0, dotPosition); // �������� ������� �� �����
        counterLogString = std::stoi(charactersBeforeDot);
        // std::cout << charactersBeforeDot << std::endl;

    }

    std::string readLog() {
        std::shared_lock<std::shared_mutex> lock(mutex);
        std::string logContent;
        std::string line;
        file.clear(); // ������� ������ ������
        file.seekg(0); // ���������� ��������� ������ � ������ �����
        while (std::getline(file, line)) { // ��������� ������ �� �����
            logContent += line + "\n"; // ��������� ��������� ������ � ������� ����
        }
        return logContent;
    }

private:
    std::fstream file;
    std::shared_mutex mutex;
};
