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
                    // Start Xvfb for graphical applications
                    sh 'Xvfb :99 -screen 0 1024x768x24 &'
                    env.DISPLAY = ':99'

                    // Build the Docker image
                    docker.build("${DOCKER_IMAGE}:${DOCKER_TAG}")

                    // Run the Docker container with X11 forwarding
                    sh """
                        xhost +local:root
                        docker run -d --name minesweeper-display \
                            -e DISPLAY=${DISPLAY} \
                            -v /tmp/.X11-unix:/tmp/.X11-unix \
                            ${DOCKER_IMAGE}:${DOCKER_TAG}
                    """

                    // Wait for user input to stop the game
                    input message: 'Minesweeper is now running. Press "Proceed" to stop the game and continue the pipeline.'

                    // Stop and remove the container
                    sh """
                        docker stop minesweeper-display
                        docker rm minesweeper-display
                        xhost -local:root
                    """
                        
                    echo "Minesweeper display test completed"
                }
            }
        }
        /*
        stage('Release'){
            input message: 'Deploy to production?'
            script {
                // Push the Docker image to a registry if necessary
                // docker.withRegistry('https://your-registry-url', 'credentials-id') {
                //     docker.image("${DOCKER_IMAGE}:${DOCKER_TAG}").push()
                // }

                // Use Octopus CLI to create a release and deploy to production
                sh """
                octo create-release --project ${OCTOPUS_PROJECT} \
                    --server ${OCTOPUS_SERVER} \
                    --apiKey ${OCTOPUS_API_KEY} \
                    --version ${DOCKER_TAG} \
                    --deployto ${OCTOPUS_ENVIRONMENT} \
                    --progress
                """
            }    
        }
        
        stage('Monitor and Alert'){
            steps{
                // Set up basic health check
                sh "while true; do curl -f http://localhost:8080 || echo 'Minesweeper is down!' | mail -s 'Minesweeper Alert' your-email@example.com; sleep 300; done &"
            }
        }
        */
    }
    
    post {
        always {
            junit allowEmptyResults: true, testResults: 'test_results.xml'
        }
        success {
            archiveArtifacts artifacts: 'minesweeper', fingerprint: true
        }
    }
}
