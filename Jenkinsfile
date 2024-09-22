pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'g++ -O main.cpp lib.cpp -o minesweeper -lsfml-graphics -lsfml-window -lsfml-system'
            }
        }
        stage('Test') {
            steps {
                sh 'g++ -std=c++11 -o test_minesweeper test_minesweeper.cpp lib.cpp -lgtest -lgtest_main -pthread'
                sh './test_minesweeper --gtest_output="xml:test_results.xml"'
            }
        }
        stage('Code Quality Analysis') {
            steps {
                sh 'cppcheck --enable=all --inconclusive --xml --xml-version=2 *.cpp *.hpp 2> cppcheck_report.xml'
            }
        }
        stage('Deploy') {
            steps {
                sh 'docker build -t minesweeper .'
                sh 'docker run minesweeper'
            }
        }
    }
    post {
        always {
            junit allowEmptyResults: true, testResults: 'test_results.xml'
            recordIssues enabledForFailure: true, tool: cppCheck(pattern: 'cppcheck_report.xml')
        }
        success {
            archiveArtifacts artifacts: 'minesweeper', fingerprint: true
        }
    }
}