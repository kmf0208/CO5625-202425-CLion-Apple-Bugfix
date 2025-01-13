#pragma once

#include "PrototypeTests.hpp"
#include "Requirements.h"
#include "../Simulator.h"
#include <random>
#include <chrono>
#include <thread>
#include <ranges>
#include <cmath>
#include <future>
#include <sstream>
#include <iostream>


template<class T>
int hammingDistance(T& a, T& b) {
	int distance = 0;
	auto aptr = a.begin();
	auto bptr = b.begin();
	for (; aptr != a.end() && bptr != b.end(); ++aptr, ++bptr)
		distance += *aptr == *bptr ? 0 : 1;
	distance += std::labs(a.size() - b.size());
	return distance;
}

bool inline isGame(std::list<std::string>& game, std::vector<std::string>& data) {
	char last = 0;
	for (auto i = game.begin(); i != game.end(); ++i) {
		if (last > 0 && (*i)[0] != last)
			return false;
		if (!std::binary_search(data.begin(), data.end(), *i))
			return false;
		if (std::find(game.begin(), i, *i) != i) 
			return false;
		if ((*i)[0] == 0)
			return false;
		last = (*i).back();
	}
	return true;
}

template<class T>
	requires hasSeed<T>
bool checkPrototypeSeed(std::list<std::string>& messages){
	std::vector<std::string> data = generateRandomStringFile(1000, 10, test_file);
	T prototype(test_file);
	std::vector<std::string> cities(data.size()), cities2(data.size());
	int seed = rand();
	prototype.seed(seed);
	for (int i = 0; i < data.size(); ++i) {
		cities[i] = prototype.getCity(data[i][0]);
		if (cities[i].empty()) {
			messages.push_back("Unable to run seed tests due to invalid cities being returned");
			return false;
		}
	}
	prototype.seed(seed);
	for (int i = 0; i < data.size(); ++i) {
		if (prototype.getCity(data[i][0]) != cities[i]) {
			messages.push_back("Prototype seed method is not producing consistent outputs");
			return false;
		}
	}
	prototype.seed(seed+1);
	int count = 0;
	for (int i = 0; i < data.size(); ++i) {
		if (prototype.getCity(data[i][0]) == cities[i]) {
			++count;
		}
	}
	if (count > data.size() / 2) {
		messages.push_back("Prototype seed method is not producing varied outputs with different seeds");
		return false;
	}
	return true;
}

template<class T>
bool checkPrototypeSeed(std::list<std::string>& messages) {
	return false;
}

template<class T>
	requires hasRun<T>
bool checkRunConsistency(std::list<std::string>& messages) {
	std::vector<std::string> data = generateRandomStringFile(1500, 7, test_file);
	T s;
	int seed = std::random_device()();
	char c = static_cast<char>(std::random_device()()%26 + 'a');

	std::packaged_task<std::list<std::string>(Simulator&, std::string, char, int)> task([](T& s,std::string f,char c, int seed) { return s.run(f,c,seed); });
	std::future<std::list<std::string> > f = task.get_future();
	std::thread t(std::move(task), std::ref(s), test_file, c, seed);
	while (f.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
		srand(std::random_device()());
	}
	auto results = f.get();
	t.join();

	if (!isGame(results, data)) {
		messages.push_back("Simulation producing invalid games");
		return false;
	}

	auto results2 = s.run(test_file, c, seed);
	
	if (results.size() != results2.size()) {
		messages.push_back("Simulations are not reproducible");
		return false;
	}

	auto j = results2.begin();
	for (auto i : results) {
		if (i != *j) {
			messages.push_back("Simulations are not reproducible");
			return false;
		}
		++j;
	}
	return true;
}

template<class T>
bool checkRunConsistency(std::list<std::string>& messages) {
	return false;
}

template<hasRun T>
	requires hasRun<T>
bool checkRunVariety(std::list<std::string>& messages) {
	std::vector<std::string> data = generateRandomStringFile(1000, 7, test_file);
	T s;
	char c = data[std::random_device()() % data.size()][0];
	for (int i = 0; i < 3; i++) {
		auto result = s.run(test_file, c, std::random_device()());
		auto result2 = s.run(test_file, c, std::random_device()());
		if (result.size() < 8 || result2.size() < 8) {
			c = data[std::random_device()() % data.size()][0];
			i--;
			continue;
		}
		if (hammingDistance(result, result2) > result.size() * 0.5) {
			return true;
		}
	}
	messages.push_back("Lack of variation between simulations");
	return false;
}

template<class T>
bool checkRunVariety(std::list<std::string>& messages) {
	return false;
}

template<class T>
	requires hasBatch<T> && hasGetResults<T>
bool checkBatchValidity(std::list<std::string>& messages) {
	int msgs = messages.size();
	std::vector<std::string> data = generateRandomStringFile(1000, 7, test_file);
	T s;
	int seed = std::random_device()();
	auto now = std::chrono::high_resolution_clock::now();
	double now_ = s.batch(test_file, std::thread::hardware_concurrency()*2, seed);
	std::chrono::duration<double> t = std::chrono::high_resolution_clock::now() - now;
	double batchTime = t.count();
	
	auto resultVector = s.getResults();
	if (resultVector.empty()) {
		messages.push_back("Results not available via getResults() after batch is called");
		return false;
	}
	for (auto &i : resultVector) {
		if (!isGame(i, data)) {
			messages.push_back("Batch simulation producing invalid games");
			break;
		}
	}
	
	if (now_ / batchTime < 0.95) {
		std::stringstream ss;
		ss << "Accuracy of timings for batch() measured at " << now_ / batchTime << " (threshold 0.95)";
		messages.push_back(ss.str());
	}

	if (messages.size() > msgs)
		return false;

	return true;
}

template<class T>
bool checkBatchValidity(std::list<std::string>& messages) {
	return false;
}

template<class T>
	requires hasBatch<T>&& hasGetResults<T>
bool checkBatchConsistency(std::list<std::string>& messages) {
	int msgs = messages.size();
	std::vector<std::string> data = generateRandomStringFile(1000, 7, test_file);
	T s;
	int seed = std::random_device()();
	
	std::packaged_task< double(T&, std::string, int, int)> task([](T& s, std::string f, int k, int seed) { return s.batch(f, k, seed); });
	std::future<double> f = task.get_future();
	std::thread t(std::move(task), std::ref(s), test_file, std::thread::hardware_concurrency()*26, seed);
	while (f.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
		srand(std::random_device()());
	}
	t.join();
	s.batch(test_file, std::thread::hardware_concurrency()*26, seed);

	auto results = s.getResults();
	for(int i=0; i< std::thread::hardware_concurrency()*26; ++i)
		if (results[i] != results[i + std::thread::hardware_concurrency()*26]) {
			messages.push_back("Batch simulations are not reproducible");
			break;
		}

	for (int i = 0; i < std::thread::hardware_concurrency()*2 - 1; ++i) {
		if (std::max(results[i].size(), results[i + 1].size()) > 5 && hammingDistance(results[i], results[i + 1]) < 0.5 * std::max(results[i].size(),results[i+1].size())) {
			messages.push_back("Batch simulations are insufficiently varied");
			break;
		}
	}

	int first_chars = 0;
	for (int i = 0; i < std::thread::hardware_concurrency() * 2; ++i)
		for (int j = 0; j < i; ++j)
			if (!results[i].empty() && !results[j].empty() && results[i].front()[0] == results[j].front()[0])
				first_chars++;
	if (first_chars == (std::thread::hardware_concurrency() * 2) * (std::thread::hardware_concurrency() * 2-1)/2.0) {
		messages.push_back("Batch simulations should start with different characters");
	}
	int empties = 0;
	for (auto& i : results)
		empties += (i.empty() ? 1 : 0);
	if(empties >= results.size()/2)
		messages.push_back("Batch simulations should produce nonempty games");
	if (messages.size() > msgs)
		return false;
	return true;
}

template<class T>
bool checkBatchConsistency(std::list<std::string>& messages) {
	return false;
}

int doSimulatorTests(std::ostream& out) {
	out << "\n\033[0;1;4mSimulator\033[0m:\n\n\033[33m";
	if (!hasSeed<Prototype>)
		out << "\tPrototype seed method has no compatible declaration\n";
	if (!hasRun<Simulator>)
		out << "\tSimulator run method has no compatible declaration\n";
	if (!hasBatch<Simulator>)
		out << "\tSimulator batch method has no compatible declaration\n";
	if (!hasGetResults<Simulator>)
		out << "\tgetResults method has no compatible declaration\n";
	if (allSimulatorMethods<Simulator>)
		out << "\t\033[32mAll methods have a compatible declaration\n";

	std::list<std::string> messages;
	int score = 0;
	score += 10 * checkPrototypeSeed<Prototype>(messages);
	score += 15 * (checkRunConsistency<Simulator>(messages) & checkRunVariety<Simulator>(messages));
	score += 10 * checkBatchValidity<Simulator>(messages);
	score += 5 * checkBatchConsistency<Simulator>(messages);
	messages.sort();
	auto it = std::unique(messages.begin(), messages.end());
	messages.erase(it, messages.end());
	for (auto i : messages) {
		out << "\t\033[31m" << i << "\n";
	}
	if (messages.empty()) {
		out << "\t\033[32mNo implementation issues\n";
	}
	return score;
}