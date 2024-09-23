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
                    // Calculate build duration or other metrics as needed
                    def buildDuration = currentBuild.duration / 1000 // in seconds
                    
                    // Send metric to Datadog
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

                    // Query Datadog for metrics and save as artifact
                    sh """
                        curl -X GET "https://api.datadoghq.com/api/v1/query?from=${System.currentTimeMillis()/1000 - 3600}&to=${System.currentTimeMillis()/1000}&query=avg:jenkins.build.duration{job:${JOB_NAME}}" \
                        -H "DD-API-KEY: ${DATADOG_API_KEY}" \
                        -H "DD-APPLICATION-KEY: ${DATADOG_APP_KEY}" \
                        -o datadog_metrics.json
                    """
                    archiveArtifacts artifacts: 'datadog_metrics.json', fingerprint: true

                    // Check for active alerts
                    def alertsResponse = sh(script: """
                        curl -X GET "https://api.datadoghq.com/api/v1/monitor/search?query=status:Alert" \
                        -H "DD-API-KEY: ${DATADOG_API_KEY}" \
                        -H "DD-APPLICATION-KEY: ${DATADOG_APP_KEY}"
                    """, returnStdout: true)

                    def alerts = readJSON text: alertsResponse
                    if (alerts.monitors.size() > 0) {
                        echo "Warning: There are active Datadog alerts. Please check Datadog dashboard."
                    }
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