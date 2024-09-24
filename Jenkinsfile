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
                script {
                    def testOutput = sh(script: './test_minesweeper', returnStdout: true).trim()
                    echo testOutput
                    writeFile file: 'test_output.txt', text: testOutput
                    archiveArtifacts artifacts: 'test_output.txt', fingerprint: true
                }
            }
        }
        stage('Code Quality Analysis') {
            steps {
                script {
                    echo 'Running Cppcheck analysis...'
                    sh 'cppcheck --enable=all --xml . 2> cppcheck_result.xml'
                    archiveArtifacts artifacts: 'cppcheck_result.xml', fingerprint: true

                    echo 'Running SonarQube analysis...'
                    def scannerHome = tool 'SonarQube'

                    withSonarQubeEnv(credentialsId: 'SonarQube', installationName: 'SonarQube') {    
                        sh "${scannerHome}/bin/sonar-scanner -X"
                    }
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
                        def appName = 'Minesweeper'
                        def deploymentGroup = 'MinesweeperDeploymentGroup'
                        
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
                    def buildDuration = currentBuild.duration / 1000 // in seconds
                    
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

                    def report = """
                    Build Metrics Report
                    ====================
                    Job: ${JOB_NAME}
                    Build Number: ${BUILD_NUMBER}
                    Status: ${buildStatus}
                    Duration: ${buildDuration} seconds
                    """

                    writeFile file: 'build_metrics_report.txt', text: report
                    archiveArtifacts artifacts: 'build_metrics_report.txt', fingerprint: true
                }
            }
        } 
    }
    
    post {
        success {
            archiveArtifacts artifacts: 'minesweeper', fingerprint: true
        }
    }
}