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
                    // Clean up any existing container
                    sh '''
                        if [ $(docker ps -q -f name=minesweeper-display) ]; then
                            docker stop minesweeper-display
                            docker rm minesweeper-display
                        fi
                    '''
                    
                    // Prepare target environment
                    sh '''
                        CONTAINER_DISPLAY="0"
                        CONTAINER_HOSTNAME="xterm"

                        mkdir -p display/socket
                        touch display/Xauthority

                        DISPLAY_NUMBER=$(echo $DISPLAY | cut -d. -f1 | cut -d: -f2)
                        AUTH_COOKIE=$(xauth list | grep "^$(hostname)/unix:${DISPLAY_NUMBER} " | awk '{print $3}')

                        xauth -f display/Xauthority add ${CONTAINER_HOSTNAME}/unix:${CONTAINER_DISPLAY} MIT-MAGIC-COOKIE-1 ${AUTH_COOKIE}

                        socat UNIX-LISTEN:display/socket/X${CONTAINER_DISPLAY},fork TCP4:localhost:60${DISPLAY_NUMBER} &
                    '''

                    // Launch the container
                    sh """
                        docker run -it --rm \
                            -e DISPLAY=:${CONTAINER_DISPLAY} \
                            -e XAUTHORITY=/tmp/.Xauthority \
                            -v ${WORKSPACE}/display/socket:/tmp/.X11-unix \
                            -v ${WORKSPACE}/display/Xauthority:/tmp/.Xauthority \
                            --hostname ${CONTAINER_HOSTNAME} \
                            ${DOCKER_IMAGE}:${DOCKER_TAG}
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
