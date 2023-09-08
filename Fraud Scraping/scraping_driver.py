from getpass import getpass
from TwitterBotScraper import TwitterBotScraper
import time
import pandas as pd
from RedditBotScrape import RedditBotScraper
import os
from FacebookBotScraper import FacebookBotScraper


# Pass in filename such as example.csv
def append_data_to_sheet(df, output_path):
    try:
        df.to_csv(output_path, mode='a', header=not os.path.exists(output_path))
        print('Appended dataframe to', output_path, '.')
    except:
        df.to_csv(output_path, mode='w')
        print('Wrote dataframe to new csv file at', output_path, '.')


def twitter_process():
    # First enter username and password
    username = input("Enter your Twitter username: ")
    password = getpass("Enter your Twitter password: ")

    # Initialize the TwitterBotScraper
    twitterBot = TwitterBotScraper(username, password)
    success = twitterBot.login_twitter_default()
    time.sleep(10)

    tweet_table = pd.DataFrame()
    if success:
        tweet_table = twitterBot.get_tweets("bitcoin", 20)
        print(tweet_table.head(10))

        # Quit twitter session
        print('Exiting Twitter session with data')
        twitterBot.quit_twitter_session()
    else:
        print('Forcing exit of Twitter session')
        twitterBot.quit_twitter_session()
    return tweet_table


def reddit_process():
    redditBot = RedditBotScraper()

    # Get Reddit posts
    reddit_table = redditBot.get_reddit_posts(5)
    print(reddit_table.head(10))

    # Quit Reddit session
    print('Exiting Reddit session with data')
    redditBot.quit_reddit_session()

    return reddit_table


def facebook_process():
    # First enter username and password
    username = input("Enter your Facebook username: ")
    password = getpass("Enter your Facebook password: ")

    # Initialize the TwitterBotScraper
    facebookBot = FacebookBotScraper(username, password)
    success = facebookBot.login_facebook_default()
    time.sleep(10)

    facebook_table = pd.DataFrame()
    if success:
        facebook_table = facebookBot.get_facebook_groups("bitcoin", 20)
        print(facebook_table.head(10))

        # Quit twitter session
        print('Exiting Facebook session with data')
        facebookBot.quit_facebook_session()
    else:
        print('Forcing exit of Twitter session')
        facebookBot.quit_facebook_session()
    return facebook_table


def main():
    start_time = time.time()
    # twitter_df = twitter_process()
    # reddit_df = reddit_process()
    facebook_df = facebook_process()
    end_time = time.time()

    # Save dataframe to excel sheet
    # append_data_to_sheet(twitter_df, 'tweets.csv')
    append_data_to_sheet(facebook_df, 'facebook.csv')



    # print('Twitter process took', round((end_time - start_time) / 60, 2), 'minutes for', twitter_df.shape[0], 'results')
    # print('Reddit process took', round((end_time-start_time)/60, 2), 'minutes to get', reddit_df.shape[0], 'results')
    print('Facebook process took', round((end_time-start_time)/60, 2), 'minutes to get', facebook_df.shape[0], 'results')


if __name__ == '__main__':
    main()
