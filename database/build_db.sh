http -a 'admin:changeit' PUT localhost:8080/db desc='Inital DB Creation'
http -a 'admin:changeit' PUT localhost:8080/db/fundamentals desc='Fundamentals Collection Creation'
http -a 'admin:changeit' PUT localhost:8080/db/watchlist desc='Watchlist Collection Creation'
http -a 'admin:changeit' PUT localhost:8080/db/stock desc='Stock Collection Creation'


