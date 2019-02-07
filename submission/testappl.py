# with open("aapl.o.csv") as infile:
#     for line in infile:
#         print(line)
import requests
print(requests.get("http://cnbc.com/finance/?page=1").text)