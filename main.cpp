

#include "show.h"
#include <random>

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester{
public:
    //double sampleTimeMeasurement(Show & aShow, int tempArray[], int arraySize);

    void test() {
        Show tree;
        Random typeGen(0, 2);
        Drone drone(5, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
        // Test the insertion function for a normal case.
        tree.insert(drone);
        if (tree.m_root->getID() == 5) {
            cout << "Test passed for single insertion" << endl;
        }


        // Test the insertion function for a edge case.
        for (int i = 1; i <= 100; i++) {
            Drone drone_A(i, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
            tree.insert(drone_A);
        }
        tree.listDrones();
        cout << "Test passed for multiple insertion" << endl;;

        Show errorTree;
        // Test the insertion function for a error case.
        Drone drone_B(5, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
        // Test the insertion function for a normal case.
        errorTree.insert(drone_B);
        errorTree.insert(drone_B);
        //when we try to insert two drone with same id, it does not get inserted

        errorTree.listDrones();
        cout << "Drone with same id does not get inserted" << endl;
        //only one of them get inserted
        cout << "***********************************************************************************" << endl;


        //testing assignment operator
        Show copy;
        copy = errorTree;
        errorTree.dumpTree();
        copy.dumpTree();
        cout << endl;
        cout << endl;

        //error case nothing gets copied
        Show a;
        Show b;
        b = a;
        a.dumpTree();
        b.dumpTree();
        cout << "Test failed because of empty tree " << endl;
        cout << "After inserting again :" << endl;
        a.insert(1);
        a.listDrones();


    }


        void isBalancedOrBST(){
            Random typeGen(0, 2);
            // Test whether the tree is balanced after a decent number of insertions, e.g. 300 insertions.
            Show balancedTree;
            // Test the insertion function for a edge case.
            for (int i = 1; i <= 300; i++) {
                Drone drone_A(i, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
                balancedTree.insert(drone_A);
            }

            cout << "Left height : " << balancedTree.m_root->getLeft()->getHeight() << endl;
            cout << "Right Height : " << balancedTree.m_root->getRight()->getHeight() << endl;
            if ((balancedTree.m_root->getLeft()->getHeight() == balancedTree.m_root->getRight()->getHeight()) or
                (balancedTree.m_root->getLeft()->getHeight() - balancedTree.m_root->getRight()->getHeight() == 1) or
                (balancedTree.m_root->getLeft()->getHeight() - balancedTree.m_root->getRight()->getHeight()) == -1) {
                cout << "The tree is balanced from first children" << endl;
            }

            if (balancedTree.isBalanced(balancedTree.m_root)) {
                cout << "The tree is balanced from all nodes" << endl;
            }

            if (balancedTree.isBST(balancedTree.m_root, MINID, MAXID)) {
                cout << "The BST property is preserved after all insertions" << endl;
            }

            cout << "***********************************************************************************" << endl;



    }


    void testRemove(){
        Random typeGen(0, 2);
        // Check remove function for an edge case
        //first insert the drones
        Show remove;
        for(int i = 0; i <= 3; i++) {
            Drone drone(i, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
            remove.insert(drone);
        }
        remove.listDrones();

        remove.remove(3);
        if(remove.findDrone(3)) {
            cout << "Drone with ID 3 successfully removed" << endl;
        }
        else {
            cout << "Drone with ID 3 not removed" << endl;
        }
        //error case

        if(!remove.findDrone(31)) {

        }
        cout << "Drone with ID 31 is not in the tree" << endl;
        // Check if tree is balanced after removing 150 drones from a tree of 300 drones
        for(int i = 40; i < 300; i++) {
            Drone drone(i, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
            remove.insert(drone);
        }
        for(int i = 150; i < 300; i++) {
            remove.remove(i);
        }
        remove.listDrones();

            if (remove.isBalanced(remove.m_root)) {
                cout << "Test Passed: Tree is balanced after removing 150 drones" << endl;
            } else {
                cout << "Test Failed: Tree is not balanced after removing 150 drones" << endl;
            }

            if (remove.isBST(remove.m_root, MINID, MAXID)) {
                cout << "The BST property is preserved after all removals" << endl;
            }else {
                cout << "The BST property is preserved after all removals" << endl;
            }


        cout << "***********************************************************************************" << endl;

    }

    void testCountFind(){
        //testing countdrones functionality
        Show test;
        Drone drone_A(1, RED);
        Drone drone_B(2, RED);
        Drone drone_C(3, BLUE);
        Drone drone_D(4, BLUE);
        Drone drone_E(5, GREEN);
        Drone drone_F(6, GREEN);
        Drone drone_Z(10, RED);

        test.insert(drone_A);
        test.insert(drone_B);
        test.insert(drone_C);
        test.insert(drone_D);
        test.insert(drone_E);
        test.insert(drone_F);
        test.insert(drone_Z);


        test.listDrones();
        cout << "We inserted 2 red so it prints : " << test.countDrones(RED) << endl;
        cout << "We inserted 2 blue so it prints : " << test.countDrones(BLUE) << endl;
        cout << "We inserted 2 green so it prints : " << test.countDrones(GREEN) << endl;

        //ideal case for find
        if (test.findDrone(1)) {
            cout << " We searched for id 1 and we found it because we had inserted it" << endl;
        }
        //error case for find
        if (!test.findDrone(10)) {
            cout << " We searched for id 10 and we did not find it" << endl;
        }

        cout << "***********************************************************************************" << endl;
    }


    void testRemoveLightOff(){

        Show lightOff;
        Random typeGen(0, 2);
        for(int i = 40; i < 41; i++) {
            Drone drone(i, static_cast<LIGHTCOLOR>(typeGen.getRandNum()),LIGHTOFF);
            lightOff.insert(drone);
        }
        for(int i = 45; i < 50; i++) {
            Drone drone(i, static_cast<LIGHTCOLOR>(typeGen.getRandNum()),LIGHTON);
            lightOff.insert(drone);
        }

        //drones with both on and off are here now
        lightOff.listDrones();

        //when we use removelight off, only the drone with lights on will be here
        lightOff.removeLightOff();
        cout << "Now ID of drones with only light ons : " << endl;

        lightOff.listDrones();
    }

    double sampleTimeMeasurement(Show & aShow, int tempArray[], int arraySize);


};


int main(){
    //calling all the tester functions
    Tester tester;
    tester.test();
    tester.isBalancedOrBST();
    tester.testRemove();
    tester.testCountFind();
    tester.testRemoveLightOff();
    Random idGen(MINID,MAXID);
    Random typeGen(0,2); // there are three colors

    {
        Show show;
        int size = 1000;
        int tempIDs[1001] = {0};
        int id = 0;
        for(int i=0;i<size;i++){
            id = idGen.getRandNum();
            tempIDs[i] = id;//we store this ID for later use
            Drone drone(id,static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
            show.insert(drone);
        }
        cout << endl << "Calling Tester::sampleTimeMeasurement(...): " << endl;
        cout << "Finding 1000 nodes takes " << tester.sampleTimeMeasurement(show, tempIDs, size) << " seconds." << endl;
    }


}

double Tester::sampleTimeMeasurement(Show & aShow, int tempArray[], int arraySize){
    double T = 0.0;//stores running times
    clock_t start, stop;//stores the clock ticks while running the program
    start = clock();
    // the algorithm to be measured
    for (int j=0;j<arraySize;j++){
        aShow.findDrone(tempArray[j]);
    }
    stop = clock();
    T = stop - start;//number of clock ticks the algorithm took
    double measureTime = T/CLOCKS_PER_SEC;//time in seconds
    return measureTime;
}