# Creates a class called Twitter Bot to access Twitter
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.edge.options import Options as EdgeOptions
from selenium.webdriver.common.keys import Keys
import time
import pandas as pd
import re


class TwitterBotScraper:
    def __init__(self, username, password):
        options = EdgeOptions()
        options.add_experimental_option('excludeSwitches', ['enable-logging'])
        self.driver = webdriver.Edge(options=options)
        self.username = username
        self.password = password

    def login_twitter_default(self):
        passed_run = False
        # initialize the driver
        self.driver.maximize_window()
        try:
            # Open up twitter
            self.driver.get("https://twitter.com/i/flow/login")

            element = WebDriverWait(self.driver, 10).until(
                EC.element_to_be_clickable((By.CSS_SELECTOR, '[name="text"]'))
            )
            element.send_keys(self.username)  # Input username

            # PASSWORD INPUT
            WebDriverWait(self.driver, 10).until(
                EC.element_to_be_clickable((By.XPATH, "//span[text()='Next']"))
            ).click()

            passwordLogin = WebDriverWait(self.driver, 10).until(
                EC.element_to_be_clickable((By.NAME, 'password'))
            )
            passwordLogin.send_keys(self.password)
            passwordLogin.send_keys(Keys.RETURN)
            passed_run = True
        except:
            passed_run = False

        return passed_run

    def clean_tweets(self, tweets):
        for i in range(len(tweets)):
            tweets[i] = ' '.join(re.sub("(@[A-Za-z0-9]+)|([^0-9A-Za-z \t]) | (\w+:\ / \ / \S+)", " "
                                        , tweets[i]).split())
        return tweets

    def get_tweets(self, search_term, num_to_pull=100):

        search_input = WebDriverWait(self.driver, 10).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, "input[data-testid='SearchBox_Search_Input']"))
        )
        search_input.send_keys(search_term)
        search_input.send_keys(Keys.RETURN)
        WebDriverWait(self.driver, 10).until(
            EC.element_to_be_clickable((By.LINK_TEXT, 'Latest'))
        ).click()

        time.sleep(10)

        user_tags = []
        timestamps = []
        tweets = []

        articles = self.driver.find_elements(By.XPATH, "//article[@data-testid='tweet']")
        while True:
            for article in articles:
                try:
                    UserTag = WebDriverWait(self.driver, 10).until(
                        EC.visibility_of(article.find_element(By.XPATH, ".//div[@dir='ltr']"))
                    )
                    user_tags.append(UserTag.text)
                except:
                    user_tags.append('')

                try:
                    TimeStamp = WebDriverWait(self.driver, 10).until(
                        EC.visibility_of(article.find_element(By.XPATH, "//time"))
                    )
                    timestamps.append(TimeStamp.get_attribute('datetime'))
                except:
                    timestamps.append('')

                try:
                    Tweet = WebDriverWait(self.driver, 10).until(
                        EC.visibility_of(article.find_element(By.XPATH, ".//div[@data-testid='tweetText']"))
                    )
                    tweets.append(Tweet.text)
                except:
                    tweets.append('')

            self.driver.execute_script('window.scrollTo(0,document.body.scrollHeight);')
            articles = self.driver.find_elements(By.XPATH, "//article[@data-testid='tweet']")
            tweets = list(set(tweets))
            if len(tweets) > num_to_pull:  # Break when a certain number of tweets have been read
                break

        # Clean the tweets and then pass the values into a dataframe
        tweets = self.clean_tweets(tweets)
        df = pd.DataFrame(zip(user_tags, timestamps, tweets),
                          columns=['UserTags', 'TimeStamps', 'Tweets'])
        return df

    def quit_twitter_session(self):
        self.driver.quit()
