pipeline {
    agent any
    
    environment {
        DOCKER_IMAGE = 'minesweeper'
        DOCKER_TAG = "${env.BUILD_NUMBER}"
        AWS_DEFAULT_REGION = 'your-region'
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
                archiveArtifacts artifacts: 'test_results.xml', fingerprint: true
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
                    sh 'zip -r minesweeper.zip minesweeper assets'
                    
                    docker.build("${DOCKER_IMAGE}:${DOCKER_TAG}")
                    sh "docker stop minesweeper-deploy || true"
                    sh "docker rm minesweeper-deploy || true"
                    sh "docker run -d --name minesweeper-deploy -p 8081:80 ${DOCKER_IMAGE}:${DOCKER_TAG}"
                    echo "Minesweeper download page available at http://localhost:8081"

                }
            }
        }
        
        stage('Release'){
            steps{
                script {
                    withCredentials([[$class: 'AmazonWebServicesCredentialsBinding', credentialsId: 'aws-credentials']]) {
                        // Define your application and deployment group
                        def appName = 'Minesweeper'
                        def deploymentGroup = 'MinesweeperDeploymentGroup'
                        
                        // Create a new revision
                        sh 'zip -r mineSweeper.zip minesweeper.zip index.html appspec.yml scripts/'
                        sh 'aws s3 cp mineSweeper.zip s3://sit223task62minesweeper/mineSweeper.zip --region ap-southeast-2'
            
                        sh '''
                            aws deploy create-deployment \
                                --application-name Minesweeper \
                                --deployment-group-name MinesweeperDeploymentGroup \
                                --s3-location bucket=sit223task62minesweeper,key=mineSweeper.zip,bundleType=zip \
                                --region ap-southeast-2
                        '''

                        sh '''
                            ssh -o StrictHostKeyChecking=no ec2-user@172.31.15.199 "chmod +x /var/www/html/scripts/start_server.sh"
                        '''
                    }

                }
                
            }
        }
        /*
        stage('Monitor and Alert'){
            steps{

            }
        }*/
        
    }
    
    post {
        always {
            junit allowEmptyResults: true, testResults: 'test_results.xml'}
        success {
            archiveArtifacts artifacts: 'minesweeper', fingerprint: true
        }
    }
}