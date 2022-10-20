import joblib, os


# DatabasePath : PKL dosyasının olduğu tam konum
# OverallQual  : Malzeme Kalitesi       [1-10]
# GrLiveArea   : Yaşam alanı m^2      [334-5642]
# GarageCars   : Araç Kapasitesi        [0-4]
# GarageArea   : Garaj m^2          [0-(160-1418)]
# FullBath     : Banyo Sayısı           [0-3]
# TotRmsAbvGrd : Toplam Odas Sayısı     [2-14]
# YearBuilt    : Yapım Yılı           [1872-2010]
# TotalSF      : Toplam Sofa alanı m^2 [334-10802]
# Fireplaces   : Şömine sayısı          [0-3]
# Neighborhood : "Ames" şehrin sınırlarındaki fiziksel konumu [0-24]
def PredictHomeValue(DatabasePath, OverallQual, GrLiveArea, GarageCars, GarageArea, FullBath,
                     TotRmsAbvGrd, YearBuilt, TotalSF, Fireplaces, Neighborhood):
    model = joblib.load(DatabasePath)

    house_to_value = [
        OverallQual,
        GrLiveArea,
        GarageCars,
        GarageArea,
        FullBath,
        TotRmsAbvGrd,
        YearBuilt,
        TotalSF,
        Fireplaces,
        Neighborhood
        ]
    
    homes_to_value = [
    house_to_value
    ]
    
    predicted_home_values = model.predict(homes_to_value)
    
    predicted_value = predicted_home_values[0]
    
 #   print("This house has an estimated value of ${:,.2f}".format(predicted_value))
    return predicted_value
