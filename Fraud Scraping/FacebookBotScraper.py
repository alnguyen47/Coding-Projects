# Creates a class called Facebook Bot to access Facebook
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.edge.options import Options as EdgeOptions
from selenium.webdriver.common.keys import Keys
import time
import pandas as pd
import re


class FacebookBotScraper:
    def __init__(self, username, password):
        options = EdgeOptions()
        options.add_experimental_option('excludeSwitches', ['enable-logging'])
        self.driver = webdriver.Edge(options=options)
        self.username = username
        self.password = password

    def login_facebook_default(self):
        passed_run = False
        # initialize the driver
        self.driver.maximize_window()
        try:
            # Open up Facebook
            self.driver.get("https://www.facebook.com/login")

            element = WebDriverWait(self.driver, 10).until(
                EC.element_to_be_clickable((By.CSS_SELECTOR, '[name="email"]'))
            )
            element.send_keys(self.username)  # Input username

            # PASSWORD INPUT
            passwordLogin = WebDriverWait(self.driver, 10).until(
                EC.element_to_be_clickable((By.NAME, 'pass'))
            )
            passwordLogin.send_keys(self.password)
            passwordLogin.send_keys(Keys.RETURN)
            passed_run = True
        except:
            passed_run = False

        return passed_run

    def clean_facebook_posts(self, posts):
        for i in range(len(posts)):
            posts[i] = ' '.join(re.sub("(@[A-Za-z0-9]+)|([^0-9A-Za-z \t]) | (\w+:\ / \ / \S+)", " "
                                        , posts[i]).split())
        return posts

    def get_facebook_groups(self, search_term, num_to_pull=100):

        search_input = WebDriverWait(self.driver, 10).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, "input[aria-label='Search Facebook']"))
        )
        search_input.send_keys(search_term)
        search_input.send_keys(Keys.RETURN)

        # Get Groups
        self.driver.get('https://www.facebook.com/search/groups?q={sterm}'.format(sterm=search_term))

        link_to_posts = []

        articles = self.driver.find_elements(By.CSS_SELECTOR, '[data-visualcompletion="ignore-dynamic"]')

        # store the posts
        while True:
            for article in articles:
                try:
                    links = WebDriverWait(self.driver, 10).until(
                        EC.visibility_of(article.find_element(
                            By.XPATH, ".//a[@role='presentation']")))
                    link_to_posts.append(links.get_attribute("href"))
                except:
                    link_to_posts.append('')

            self.driver.execute_script('window.scrollTo(0,document.body.scrollHeight);')
            articles = self.driver.find_elements(By.CSS_SELECTOR, '[data-visualcompletion="ignore-dynamic"]')
            link_to_posts = list(set(link_to_posts))
            if len(link_to_posts) > num_to_pull:  # Break when a certain number of links have been read
                break

        return link_to_posts

    def quit_facebook_session(self):
        self.driver.quit()
