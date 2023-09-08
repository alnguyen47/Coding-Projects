import urllib3
import requests
import facebook

token = 'EAAyoU0jIcdUBOyxyNkp1MLsRdyj7gy9xRerKL7xfyS3RRjNP7GDrNDu1D8Q0LaHifmySLUvZB2mY3ERwsk8zamx1QMHeDoJZC4okESugCV0Wkz5AZA3u26iPA6zzdE4A6gT0hZA7KZB1GLunCtUYSFkNNeoWLpImTHVefs2L7LXxjpQHODr3sLMeZB2o7wTtuaJBfxDRoefLTEoZBZCkZCAZDZD'

graph = facebook.GraphAPI(access_token=token, version = 3.1)
events = graph.request('/search?q=Poetry&type=event&limit=10000')

print(events['data'])