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
                    sh "docker stop minesweeper-staging || true"
                    sh "docker rm minesweeper-staging || true"
                    sh """
                        docker run -d \
                        --name minesweeper-staging \
                        -e DISPLAY=\$DISPLAY \
                        -v /tmp/.X11-unix:/tmp/.X11-unix \
                        -v /etc/machine-id:/etc/machine-id:ro \
                        --network host \
                        ${DOCKER_IMAGE}:${DOCKER_TAG} bash -c "echo \$DISPLAY && xeyes || echo xeyes failed && ./minesweeper || echo minesweeper failed"
                    """
                    sh "docker logs minesweeper-staging"
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