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
                    // Build Docker image with VNC server, noVNC, and Minesweeper
                    sh """
                    docker build -t ${DOCKER_IMAGE}:${DOCKER_TAG} -f- . <<EOF
                    FROM ubuntu:20.04
                    RUN apt-get update && apt-get install -y x11vnc xvfb libsfml-dev novnc
                    COPY minesweeper /app/minesweeper
                    COPY assets /app/assets
                    COPY novnc-index.html /novnc-index.html
                    ENV DISPLAY=:0
                    CMD Xvfb :0 -screen 0 1024x768x16 & x11vnc -display :0 -forever -passwd ${VNC_PASSWORD} & /app/minesweeper & /usr/share/novnc/utils/launch.sh --vnc localhost:5900 --listen 8080
                    EOF
                    """
                    
                    // Run the Docker container
                    sh "docker run -d --name minesweeper-test -p 8081:8080 ${DOCKER_IMAGE}:${DOCKER_TAG}"
                    
                    echo "Manual testing environment is ready. Access the game at http://localhost:8081/novnc-index.html"
                    echo "The container will be available for 30 minutes for manual testing."
                    echo "After testing, approve the deployment in the Jenkins interface to proceed."
                    
                    // Wait for manual testing and approval
                    input message: 'Game is available for manual testing. Once tested, click "Proceed" to continue with deployment, or "Abort" to stop the pipeline.'
                    
                    // Stop and remove the test container
                    sh 'docker stop minesweeper-test && docker rm minesweeper-test'
                }
            }
        }
        
        stage('Release'){
            steps{
                script {
                    // Save the Docker image as a tar file
                    sh "docker save ${DOCKER_IMAGE}:${DOCKER_TAG} > minesweeper-release.tar"
                    
                    // Copy the Docker image to the deployment server
                    sh "scp -i ${DEPLOY_SERVER_KEY} minesweeper-release.tar ${DEPLOY_SERVER}:~/"
                    
                    // SSH into the deployment server and deploy the new image
                    sh """
                    ssh -i ${DEPLOY_SERVER_KEY} ${DEPLOY_SERVER} << EOF
                        docker load < minesweeper-release.tar
                        docker stop minesweeper-prod || true
                        docker rm minesweeper-prod || true
                        docker run -d --name minesweeper-prod -p 80:80 -p 8081:8080 ${DOCKER_IMAGE}:${DOCKER_TAG}
                        rm minesweeper-release.tar
                    EOF
                    """
                }

            }
        }
        stage('Monitor and Alert'){
            steps{

            }
        }
        
    }
    
    post {
        always {
            junit allowEmptyResults: true, testResults: 'test_results.xml'}
        success {
            archiveArtifacts artifacts: 'minesweeper', fingerprint: true
        }
    }
}