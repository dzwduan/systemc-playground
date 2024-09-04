# Created by : Ali Bagheri
# GitHub page: https://github.com/bagheriali2001

##### EDIT BELOW THIS LINE #####
## define variables
echo -e "\e[32mDefine variables\e[0m"
SYSTEMC_DOWNLOAD_URL="https://github.com/accellera-official/systemc/archive/refs/tags/3.0.0.tar.gz"
echo -e "\e[32m SYSTEMC_DOWNLOAD_URL = $SYSTEMC_DOWNLOAD_URL\e[0m"
SYSTEMC_FILE_NAME="systemc-3.0.0.tar.gz"
echo -e "\e[32m SYSTEMC_FILE_NAME = $SYSTEMC_FILE_NAME\e[0m"
SYSTEMC_EXTRACTED_FOLDER_NAME="systemc-3.0.0"
echo -e "\e[32m SYSTEMC_EXTRACTED_FOLDER_NAME = $SYSTEMC_EXTRACTED_FOLDER_NAME\e[0m"
SYSTEMC_INSTALLATION_DIRECTORY="$HOME"
echo -e "\e[32m SYSTEMC_INSTALLATION_DIRECTORY = $SYSTEMC_INSTALLATION_DIRECTORY\e[0m"
##### EDIT ABOVE THIS LINE #####


##### DO NOT EDIT BELOW THIS LINE #####
## installing dependencies
echo -e "\e[32mRunning: sudo apt-get update\e[0m"
sudo apt-get update
echo -e "\e[32mRunning: sudo apt install gcc build-essential -y\e[0m"
sudo apt install gcc build-essential -y

## moving to SYSTEMC_INSTALLATION_DIRECTORY
echo -e "\e[32mRunning: cd $SYSTEMC_INSTALLATION_DIRECTORY\e[0m"
cd $SYSTEMC_INSTALLATION_DIRECTORY

## downloading and extracting systemc
echo -e "\e[32mRunning: wget $SYSTEMC_DOWNLOAD_URL\e[0m"
wget $SYSTEMC_DOWNLOAD_URL
echo -e "\e[32mRunning: mkdir $SYSTEMC_EXTRACTED_FOLDER_NAME\e[0m"
mkdir $SYSTEMC_EXTRACTED_FOLDER_NAME
echo -e "\e[32mRunning: tar -xzf $SYSTEMC_FILE_NAME -C $SYSTEMC_EXTRACTED_FOLDER_NAME --strip-components=1\e[0m"
tar -xzf $SYSTEMC_FILE_NAME -C $SYSTEMC_EXTRACTED_FOLDER_NAME --strip-components=1

## installing systemc
echo -e "\e[32mRunning: cd $SYSTEMC_EXTRACTED_FOLDER_NAME\e[0m"
cd $SYSTEMC_EXTRACTED_FOLDER_NAME
echo -e "\e[32mRunning: mkdir objdir\e[0m"
mkdir objdir
echo -e "\e[32mRunning: cd objdir\e[0m"
cd objdir
echo -e "\e[32mRunning: export CXX=g++\e[0m"
export CXX=g++
echo -e "\e[32mRunning: ../configure\e[0m"
../configure
echo -e "\e[32mRunning: make -j\e[0m"
make
echo -e "\e[32mRunning: sudo make install\e[0m"
make install
echo -e "\e[32mRunning: cd ..\e[0m"
cd ..
echo -e "\e[32mRunning: rm -rf objdir\e[0m"
rm -rf objdir

## adding systemc path to bashrc
echo -e "\e[32mRunning: echo "# SystemC Install path" >> ~/.bashrc\e[0m"
echo "# SystemC Install path" >> ~/.bashrc
echo -e "\e[32mRunning: echo "export SYSTEMC_HOME=$SYSTEMC_INSTALLATION_DIRECTORY/$SYSTEMC_EXTRACTED_FOLDER_NAME" >> ~/.bashrc\e[0m"
echo "export SYSTEMC_HOME=$SYSTEMC_INSTALLATION_DIRECTORY/$SYSTEMC_EXTRACTED_FOLDER_NAME" >> ~/.bashrc
if [ -z "$LD_LIBRARY_PATH" ]; then
    echo -e "\e[32mRunning: echo "export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64" >> ~/.bashrc\e[0m"
    echo "export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64" >> ~/.bashrc
else
    echo -e "\e[32mRunning: echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SYSTEMC_HOME/lib-linux64" >> ~/.bashrc\e[0m"
    echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SYSTEMC_HOME/lib-linux64" >> ~/.bashrc
fi

echo -e "\e[32mSystemC installation is done successfully\e[0m"
echo -e "\e[34m\e[1mCreated by Ali Bagheri\e[0m"
echo -e "\e[34m\e[1mGitHub page: https://github.com/bagheriali2001\e[0m"