# Creates a class RedditBotScraper to access Reddit
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.edge.options import Options as EdgeOptions
import pandas as pd
import requests
from bs4 import BeautifulSoup


def getdata(url):  # Helper function to get html_doc
    headers = {'method': 'GET'}
    r = requests.get(url, headers=headers)
    return r.text


class RedditBotScraper:
    def __init__(self):
        options = EdgeOptions()
        options.add_experimental_option('excludeSwitches', ['enable-logging'])
        self.driver = webdriver.Edge(options=options)
        self.link_to_post = []

    def get_reddit_posts(self, search_term, num_to_pull=100):
        # Open up Reddit
        self.driver.get('https://www.reddit.com/search?q={sterm}&type=link&sort=new'.format(sterm=search_term))

        # retrieve the list of post HTML elements
        post_html_elements = self.driver.find_elements(By.CSS_SELECTOR, '[data-testid="search-post"]')

        # store the posts
        while True:
            for post_html_element in post_html_elements:
                try:
                    links = WebDriverWait(self.driver, 10).until(
                        EC.visibility_of(post_html_element.find_element(
                            By.XPATH, ".//a[@data-testid='post-title']")))
                    self.link_to_post.append(links.get_attribute("href"))
                except:
                    self.link_to_post.append('')

            self.driver.execute_script('window.scrollTo(0,document.body.scrollHeight);')
            post_html_elements = self.driver.find_elements(By.CSS_SELECTOR, '[data-testid="search-post"]')
            self.link_to_post = list(set(self.link_to_post))
            if len(self.link_to_post) > num_to_pull:  # Break when a certain number of links have been read
                break

        return self.table_reddit_posts()

    def table_reddit_posts(self):
        # Use the links to get the relevant data values
        reddit_title = []
        reddit_text = []
        username = []

        for link in self.link_to_post:
            print(link)
            html_doc = getdata(link)
            soup = BeautifulSoup(html_doc, 'html.parser')

            try:
                r_text = soup.find('div', slot='text-body').get_text()
                reddit_text.append(r_text)
                print(r_text)
            except:
                reddit_text.append('')

            try:
                uname = soup.find('span', slot='authorName').get_text()
                username.append(uname)
                print(uname)
            except:
                username.append('')

            try:
                r_title = soup.find('div', slot='title').get_text()
                reddit_title.append(r_title)
                print(r_title)
            except:
                reddit_title.append('')

        return pd.DataFrame(zip(reddit_title, reddit_text, username),
                            columns=['Reddit Post Title', 'Reddit Post Text', 'User'])

    def quit_reddit_session(self):
        self.driver.quit()
