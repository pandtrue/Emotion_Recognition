#include "naiveBayes.h"

NaiveBayes::NaiveBayes(map<int, Emotion*> learning_map_)
    :learning_map(learning_map_)
{
  init();
}

NaiveBayes::~NaiveBayes()
{
}

void NaiveBayes::init(){
  total_sentence = 0;
  total_word = 0;
  map<int, Emotion*>::iterator it;
  for(it = learning_map.begin(); it != learning_map.end(); it++){
    Emotion *emotion = it->second;

    total_sentence += emotion->sentence_count;
    emotion_prob[it->first] = emotion->sentence_count;

    map<string, WordInfo*> words_map = emotion->words_map;;
    map<string, WordInfo*>::iterator w_it;
    for(w_it = words_map.begin(); w_it != words_map.end(); w_it++){
      total_word += w_it->second->count;
    }
  }
  
  map<int, double>::iterator e_it;
  for(e_it = emotion_prob.begin(); e_it != emotion_prob.end(); e_it++){
    int emotion_num = e_it->first;
    double prob = e_it->second / (double)total_sentence;
    emotion_prob[emotion_num] = prob;
  }
}

int NaiveBayes::getEmotion(string input){
  stringstream ss;
  vector<string> words;
  string word;
  ss << input;
  while(ss >> word){
    words.push_back(word);
  }

  int ret = calNaiveBayes(words);
  return ret;
}

int NaiveBayes::calNaiveBayes(vector<string> words){
  double max_emotion_prob = 0;
  int max_emotion_num = 0;
  map<int, Emotion*>::iterator it;
  for(it=learning_map.begin(); it!=learning_map.end(); it++){
    int emotion_num = it->first;
    if(emotion_num == 0)   // skip the 'nothing-emotion'
      continue;
    double result = emotion_prob[emotion_num];
    Emotion *emotion = it->second;
    
    // test
    cout << "\nEmotion [" << emotion->name << "] : " << result << endl;

    cout << "words size : " << words.size() << endl;
    for(int i=0; i<words.size(); i++){
      if(emotion->words_map.find( words[i] ) != emotion->words_map.end()){
	//result *= emotion->words_map[ words[i] ]->count / total_word;
	result *= emotion->words_map[ words[i] ]->count;
	// test
	cout << "word : " << words[i] << ", count : " << emotion->words_map[ words[i] ]->count << endl;
      }else{
	//result *= 1 / total_word;
      }
    }
    
    cout << "result >> " << result << endl;
    

    if(result > max_emotion_prob){
      max_emotion_prob = result;
      max_emotion_num = emotion_num;
    }
  }
  cout << "\ntotal sentence : " << total_sentence << endl;
  cout << "total word : " << total_word << endl;
  return max_emotion_num;
}