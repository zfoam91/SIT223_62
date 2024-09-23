pipeline {
    agent any
    
    environment {
        DOCKER_IMAGE = 'minesweeper'
        DOCKER_TAG = "${env.BUILD_NUMBER}"
        DEPLOY_PORT = '8080'
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
                    // Create a zip file containing the executable and assets
                    sh 'zip -r minesweeper.zip minesweeper assets'
                    sh 'mv minesweeper.zip /var/www/html/'
                    sh 'cp index.html /var/www/html/'
                    docker.build("${DOCKER_IMAGE}:${DOCKER_TAG}")
                    sh "docker stop minesweeper-deploy || true"
                    sh "docker rm minesweeper-deploy || true"
                    sh "docker run -d --name minesweeper-deploy -p ${DEPLOY_PORT}:80 ${DOCKER_IMAGE}:${DOCKER_TAG}"
                    echo "Minesweeper download page available at http://localhost:${DEPLOY_PORT}"

                }
            }
        }
        /*
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
            junit allowEmptyResults: true, testResults: 'test_results.xml'}
        success {
            archiveArtifacts artifacts: 'minesweeper', fingerprint: true
        }
    }
}