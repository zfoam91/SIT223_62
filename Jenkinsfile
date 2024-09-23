pipeline {
    agent any
    
    environment {
        DOCKER_IMAGE = 'minesweeper'
        DOCKER_TAG = "${env.BUILD_NUMBER}"
    }

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
                sh 'cppcheck --enable=all --xml . 2> cppcheck_result.xml'
                archiveArtifacts artifacts: 'cppcheck_result.xml', fingerprint: true
            }
        }
        
        stage('Deploy') {
            steps {
                script {
                    docker.build("${DOCKER_IMAGE}:${DOCKER_TAG}")
                    sh "docker stop minesweeper-staging || true"
                    sh "docker rm minesweeper-staging || true"
                    sh "docker run -d --name minesweeper-staging ${DOCKER_IMAGE}:${DOCKER_TAG}"
                } 
            }
        }
        
        stage('Release'){
            steps{
                sh "docker stop firefox-vnc || true"
                    sh "docker rm firefox-vnc || true"
                    
                    // Pull and run the creack/firefox-vnc image
                    sh '''
                    docker pull creack/firefox-vnc
                    docker run -d -p 5900:5900 --name firefox-vnc -e HOME=/ creack/firefox-vnc x11vnc -forever -usepw -create
                    '''
            }
        }
        /*
        stage('Monitor and Alert'){
            steps{

            }
        }
        */
    }
    
    post {
        always {
            junit allowEmptyResults: true, testResults: 'test_results.xml'}
        success {
            archiveArtifacts artifacts: 'minesweeper', fingerprint: true
        }
    }
}