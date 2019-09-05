# Basic Reason Template

Hello! This project allows you to quickly get started with Reason and BuckleScript. If you wanted a more sophisticated version, try the `react` template (`bsb -theme react -init .`).

# Build
```
npm run build
```

# Build + Watch

```
npm run start
```


# Editor
If you use `vscode`, Press `Windows + Shift + B` it will build automatically


## GET request to SNCF API example
`https://sncf-simulateur-api-prod.azurewebsites.net/api/Calendar/PARIS%20(intramuros)/RENNES/1/2019-09-05`
This request requires a header `ValidityToken` that can be found in the body content of the request:
`https://simulateur.tgvmax.fr/VSC/`

Response : 
```json
[
    {
        "date": "2019-08-26T00:00:00",
        "availability": 0,
        "weekNumber": 34
    },
    {
        "date": "2019-08-27T00:00:00",
        "availability": 0,
        "weekNumber": 34
    },
    {
        "date": "2019-08-28T00:00:00",
        "availability": 0,
        "weekNumber": 34
    },
    {
        "date": "2019-08-29T00:00:00",
        "availability": 0,
        "weekNumber": 34
    },
    {
        "date": "2019-08-30T00:00:00",
        "availability": 0,
        "weekNumber": 34
    },
    {
        "date": "2019-08-31T00:00:00",
        "availability": 0,
        "weekNumber": 34
    },
    {
        "date": "2019-09-01T00:00:00",
        "availability": 0,
        "weekNumber": 34
    },
    {
        "date": "2019-09-02T00:00:00",
        "availability": 0,
        "weekNumber": 35
    },
    {
        "date": "2019-09-03T00:00:00",
        "availability": 0,
        "weekNumber": 35
    },
    {
        "date": "2019-09-04T00:00:00",
        "availability": 0,
        "weekNumber": 35
    },
    {
        "date": "2019-09-05T00:00:00",
        "availability": 2,
        "weekNumber": 35
    },
    {
        "date": "2019-09-06T00:00:00",
        "availability": 1,
        "weekNumber": 35
    },
    {
        "date": "2019-09-07T00:00:00",
        "availability": 3,
        "weekNumber": 35
    },
    {
        "date": "2019-09-08T00:00:00",
        "availability": 1,
        "weekNumber": 35
    },
    {
        "date": "2019-09-09T00:00:00",
        "availability": 1,
        "weekNumber": 36
    },
    {
        "date": "2019-09-10T00:00:00",
        "availability": 1,
        "weekNumber": 36
    },
    {
        "date": "2019-09-11T00:00:00",
        "availability": 1,
        "weekNumber": 36
    },
    {
        "date": "2019-09-12T00:00:00",
        "availability": 1,
        "weekNumber": 36
    },
    {
        "date": "2019-09-13T00:00:00",
        "availability": 1,
        "weekNumber": 36
    },
    {
        "date": "2019-09-14T00:00:00",
        "availability": 1,
        "weekNumber": 36
    },
    {
        "date": "2019-09-15T00:00:00",
        "availability": 1,
        "weekNumber": 36
    },
    {
        "date": "2019-09-16T00:00:00",
        "availability": 1,
        "weekNumber": 37
    },
    {
        "date": "2019-09-17T00:00:00",
        "availability": 1,
        "weekNumber": 37
    },
    {
        "date": "2019-09-18T00:00:00",
        "availability": 1,
        "weekNumber": 37
    },
    {
        "date": "2019-09-19T00:00:00",
        "availability": 1,
        "weekNumber": 37
    },
    {
        "date": "2019-09-20T00:00:00",
        "availability": 1,
        "weekNumber": 37
    },
    {
        "date": "2019-09-21T00:00:00",
        "availability": 2,
        "weekNumber": 37
    },
    {
        "date": "2019-09-22T00:00:00",
        "availability": 1,
        "weekNumber": 37
    },
    {
        "date": "2019-09-23T00:00:00",
        "availability": 1,
        "weekNumber": 38
    },
    {
        "date": "2019-09-24T00:00:00",
        "availability": 1,
        "weekNumber": 38
    },
    {
        "date": "2019-09-25T00:00:00",
        "availability": 1,
        "weekNumber": 38
    },
    {
        "date": "2019-09-26T00:00:00",
        "availability": 1,
        "weekNumber": 38
    },
    {
        "date": "2019-09-27T00:00:00",
        "availability": 1,
        "weekNumber": 38
    },
    {
        "date": "2019-09-28T00:00:00",
        "availability": 2,
        "weekNumber": 38
    },
    {
        "date": "2019-09-29T00:00:00",
        "availability": 1,
        "weekNumber": 38
    },
    {
        "date": "2019-09-30T00:00:00",
        "availability": 1,
        "weekNumber": 39
    },
    {
        "date": "2019-10-01T00:00:00",
        "availability": 1,
        "weekNumber": 39
    },
    {
        "date": "2019-10-02T00:00:00",
        "availability": 1,
        "weekNumber": 39
    },
    {
        "date": "2019-10-03T00:00:00",
        "availability": 1,
        "weekNumber": 39
    },
    {
        "date": "2019-10-04T00:00:00",
        "availability": 1,
        "weekNumber": 39
    },
    {
        "date": "2019-10-05T00:00:00",
        "availability": 2,
        "weekNumber": 39
    },
    {
        "date": "2019-10-06T00:00:00",
        "availability": 0,
        "weekNumber": 39
    }
]
```
