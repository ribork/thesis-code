#include "c4.h"
#include "math.h"

C4::C4()
{
    recurrent[0] = 1.0;
    recurrent[1] = 1.0;
    recurrent[2] = 1.0;
    recurrent[3] = 1.0;
}

std::string C4::getInfo(){
    return "2 discrete and 2 continiuous without action";
}

int C4::getNumberOfGenes(){
    return 70;
}

int C4::getActionLayerStart()
{
    return 44;
}

std::string C4::getType(){
    return "4";
}

float C4::activation(float x){
    // sigmoid
    return 2.0/(1.0+exp(-2.0*x))-1.0;
}

std::vector<float> C4::compute(std::vector<float> input, std::vector<float> genome){
    std::vector<float> output;

    // prediction network
    int p_n_input_nodes = 4;
    float p_input_layer[p_n_input_nodes];
    for(int i = 0; i < p_n_input_nodes; i++)
        p_input_layer[i] = activation(genome[2*i] * input[i] - genome[2*i+1]);


    int p_n_first_hidden_nodes = 4;
    float p_first_hidden_layer[p_n_first_hidden_nodes];
    int p_first_hidden_layer_start = 8;
    for(int i = 0; i < p_n_first_hidden_nodes; i++){
        for(int j = 0; j < p_n_input_nodes; j++){
           p_first_hidden_layer[i] += genome[p_first_hidden_layer_start+(p_n_input_nodes+1)*i+j] * p_input_layer[j];
        }
        p_first_hidden_layer[i] = activation(p_first_hidden_layer[i] + genome[p_first_hidden_layer_start+(p_n_input_nodes+1)*i+p_n_input_nodes] * recurrent[i]);
        recurrent[i] = p_first_hidden_layer[i];
    }

    int p_n_output_nodes = 4;
    float p_output_layer[p_n_output_nodes];
    int p_output_layer_start = 28;
    for(int i = 0; i < p_n_output_nodes; i++){
        for(int j = 0; j < p_n_first_hidden_nodes; j++){
           p_output_layer[i] += genome[p_output_layer_start+p_n_first_hidden_nodes*i + j] * p_first_hidden_layer[j];
        }
        p_output_layer[i] = activation(p_output_layer[i]);
    }

    output.push_back(p_output_layer[0] > 0.0 ? 1.0 : 0.0);
    output.push_back(0.5 * p_output_layer[1] + 0.5);
    output.push_back(p_output_layer[2] > 0.0 ? 1.0 : 0.0);
    output.push_back(0.5 * p_output_layer[3] + 0.5);

    // action network
    int action_weights_start = 44;
    int a_n_input_nodes = 4;
    float a_input_layer[a_n_input_nodes];
    for(int i = 0; i < a_n_input_nodes; i++)
        a_input_layer[i] = activation(genome[action_weights_start+2*i] * input[i] - genome[action_weights_start+2*i+1]);

    int a_n_first_hidden_layer_nodes = 3;
    float a_first_hidden_layer[a_n_first_hidden_layer_nodes];
    int a_first_hidden_layer_start = 52;

    for(int i = 0; i < a_n_first_hidden_layer_nodes; i++){
        for(int j = 0; j < a_n_input_nodes; j++){
            a_first_hidden_layer[i] += genome[a_first_hidden_layer_start+a_n_input_nodes*i+j] * a_input_layer[j];
        }
        a_first_hidden_layer[i] = activation(a_first_hidden_layer[i]);
    }

    int a_n_output_nodes = 2;
    float a_output_layer[2];
    int a_output_layer_start = 64;

    for(int i = 0; i < a_n_output_nodes; i++){
        for(int j = 0; j < a_n_first_hidden_layer_nodes; j++){
            a_output_layer[i] += genome[a_output_layer_start+a_n_first_hidden_layer_nodes*i+j] * a_first_hidden_layer[j];
        }
        a_output_layer[i] = activation(a_output_layer[i]);
    }

    float speed = 0.5 * a_output_layer[0] + 0.5;
    float delta_angle = a_output_layer[1];

    output.push_back(speed);
    output.push_back(delta_angle);

    return output;
}
