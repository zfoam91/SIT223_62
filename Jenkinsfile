pipeline {
    agent any
    
    environment {
        DOCKER_IMAGE = 'minesweeper'
        DOCKER_TAG = "${env.BUILD_NUMBER}"
        DATADOG_API_KEY = 'cc87e1d8af609a39ef59fe2f64c11591'
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
                script {
                    sh 'cppcheck --enable=all --xml . 2> cppcheck_result.xml'
                    archiveArtifacts artifacts: 'cppcheck_result.xml', fingerprint: true
                }  
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
                    }

                }
                
            }
        }
        
        stage('Monitor and Alert'){
            steps{
                script {
                    // Calculate build duration
                    def buildDuration = currentBuild.duration / 1000 // in seconds
                    
                    // Send build duration metric to Datadog
                    sh """
                        curl -X POST "https://api.datadoghq.com/api/v1/series" \
                        -H "Content-Type: application/json" \
                        -H "DD-API-KEY: ${DATADOG_API_KEY}" \
                        -d '{
                            "series": [{
                                "metric": "jenkins.build.duration",
                                "points": [[${System.currentTimeMillis() / 1000}, ${buildDuration}]],
                                "type": "gauge",
                                "tags": ["job:${JOB_NAME}", "build:${BUILD_NUMBER}"]
                            }]
                        }'
                    """

                    // Send a Datadog event for build completion
                    def buildStatus = currentBuild.currentResult
                    sh """
                        curl -X POST "https://api.datadoghq.com/api/v1/events" \
                        -H "Content-Type: application/json" \
                        -H "DD-API-KEY: ${DATADOG_API_KEY}" \
                        -d '{
                            "title": "Jenkins Build ${JOB_NAME} #${BUILD_NUMBER}",
                            "text": "Build completed with status: ${buildStatus}\\nDuration: ${buildDuration} seconds",
                            "priority": "normal",
                            "tags": ["jenkins", "build:${BUILD_NUMBER}", "job:${JOB_NAME}", "status:${buildStatus}"],
                            "alert_type": "${buildStatus == 'SUCCESS' ? 'success' : 'error'}"
                        }'
                    """

                    // Create a simple metrics report
                    def report = """
                    Build Metrics Report
                    ====================
                    Job: ${JOB_NAME}
                    Build Number: ${BUILD_NUMBER}
                    Status: ${buildStatus}
                    Duration: ${buildDuration} seconds

                    This data has been sent to Datadog for monitoring and alerting.
                    """

                    writeFile file: 'build_metrics_report.txt', text: report
                    archiveArtifacts artifacts: 'build_metrics_report.txt', fingerprint: true
                }
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