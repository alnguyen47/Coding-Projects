from nltk import word_tokenize
import pandas as pd
from textblob import TextBlob
import re
from nltk.corpus import stopwords
import string
from nltk.stem.wordnet import WordNetLemmatizer
from nltk.stem.porter import PorterStemmer
from gensim import corpora
import gensim
import pyLDAvis.gensim
pd.options.display.max_columns = None
pd.options.display.max_rows = None


# Does sentiment analysis on text
def getTextSubjectivity(txt):
    return TextBlob(txt).sentiment.subjectivity

def getTextPolarity(txt):
    return TextBlob(txt).sentiment.polarity

def getTextScore(polarity_val):
    if polarity_val < 0:
        return "Negative"
    elif polarity_val == 0:
        return "Neutral"
    else:
        return "Positive"

def process_text(txt):
    # 1. Tokenization
    tokenized_text = word_tokenize(txt)
    # print(tokenized_text)
    tokenized_text = [re.sub(r'[^a-zA-Z]', '', word) for word in tokenized_text]

    # 2. Remove stopwords and punctuation
    exclude = set(string.punctuation)
    stop = set(stopwords.words('english'))
    stop_free = [word for word in tokenized_text if ((word not in stop) and (not word.isdigit()))]
    punc_free = [word for word in stop_free if word not in exclude]
    punc_free = [word for word in punc_free if word != '']

    # 3. Lemmatize and stem words
    lemma = WordNetLemmatizer()
    normalized = [lemma.lemmatize(word) for word in punc_free]
    porter = PorterStemmer()
    cleaned_text = [porter.stem(token) for token in normalized]
    return cleaned_text

# Returns the LDA model for a given set of words
def topic_modelling(cleaned_text):
    # Create dictionary for number of times a word appears
    word_dict = corpora.Dictionary(cleaned_text)

    # Filter out non frequent words
    # word_dict.filter_extremes(no_below=5, keep_n=10)

    # Create corpus
    corpus = [word_dict.doc2bow(text) for text in cleaned_text]

    # Define Model
    lda_model = gensim.models.ldamodel.LdaModel(corpus, num_topics=3, id2word=word_dict, passes=15)

    # Display Topics
    # lda_display = pyLDAvis.gensim.prepare(lda_model, corpus, word_dict, sort_topics=False)
    # pyLDAvis.display(lda_display)

    return lda_model, corpus


def get_topic_details(cleaned_text):
    lda_model, corpus = topic_modelling(cleaned_text)
    topic_details_df = pd.DataFrame()
    topic_num_list = []
    prop_topic_list = []
    for i, row in enumerate(lda_model[corpus]):
        row = sorted(row, key=lambda x: (x[1]), reverse=True)
        for j, (topic_num, prop_topic) in enumerate(row):
            if j == 0: # dominant topic
                wp = lda_model.show_topic(topic_num)
                topic_num_list.append(topic_num)
                prop_topic_list.append(prop_topic)
    topic_details_df = pd.DataFrame(zip(topic_num_list, prop_topic_list),
                          columns=['Dominant Topic', '%Score'])
    return topic_details_df
def main():
    # First open csv file
    twitter_df = pd.read_csv('tweets.csv')

    # Perform sentiment analysis
    twitter_df = twitter_df.dropna()
    twitter_df['Subjectivity'] = twitter_df.apply(lambda row: getTextSubjectivity(row['Tweets']), axis=1)
    twitter_df['Polarity'] = twitter_df.apply(lambda row: getTextPolarity(row['Tweets']), axis=1)
    twitter_df = twitter_df.drop(twitter_df[twitter_df['Tweets'] == ''].index)
    twitter_df['Score'] = twitter_df.apply(lambda row: getTextScore(row['Polarity']), axis=1)
    twitter_df['Tweets'] = twitter_df.apply(lambda row: process_text(row["Tweets"]), axis=1)

    twitter_df = pd.concat([twitter_df, get_topic_details(twitter_df['Tweets'].tolist())], axis='columns')
    print(twitter_df.head(5))


if __name__ == '__main__':
    main()