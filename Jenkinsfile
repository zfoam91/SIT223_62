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
                script {
                    def scannerHome = tool 'SonarQube Scanner' // Make sure the tool name matches your configuration
                    withSonarQubeEnv('SonarQube Server Name') { // Use the server name configured earlier
                        sh "${scannerHome}/bin/sonar-scanner -Dsonar.projectKey=yourProjectKey -Dsonar.sources=."
                    }
                }
            }
        }
        /*
        stage('Deploy') {
            steps {
                sh 'docker build -t minesweeper .'
                sh 'docker run minesweeper'
            }
        }
        stage('Release'){
            steps{

            }
        }
        stage('Monitor and Alert'){
            steps{

            }
        }
        */
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