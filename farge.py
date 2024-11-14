import cv2
import urllib.request
import numpy as np
import time

def nothing(x):
    pass

def detect_color(h, s, v):
    if s < 40 and v > 200:
        return "Hvit"
    elif s < 40 and v < 200:
        return "Grå"
    elif v < 40:
        return "Svart"
    elif (0 <= h < 10 or 170 <= h <= 180):
        return "Rød"
    elif 10 <= h < 30:
        return "Oransje"
    elif 30 <= h < 50:
        return "Gul"
    elif 50 <= h < 85:
        return "Grønn"
    elif 85 <= h < 130:
        return "Blå"
    elif 130 <= h < 160:
        return "Lilla"
    else:
        return "Ukjent farge"

url = 'http://172.20.10.14/cam-lo.jpg'

cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)
cv2.namedWindow("Tracking")
cv2.createTrackbar("LH", "Tracking", 20, 180, nothing)
cv2.createTrackbar("LS", "Tracking", 60, 255, nothing)
cv2.createTrackbar("LV", "Tracking", 120, 255, nothing)
cv2.createTrackbar("UH", "Tracking", 50, 180, nothing)
cv2.createTrackbar("US", "Tracking", 255, 255, nothing)
cv2.createTrackbar("UV", "Tracking", 255, 255, nothing)

while True:
    try:
        img_resp = urllib.request.urlopen(url)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        frame = cv2.imdecode(imgnp, -1)
    except Exception as e:
        print("Failed to retrieve image:", e)
        continue
    
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    l_h = cv2.getTrackbarPos("LH", "Tracking")
    l_s = cv2.getTrackbarPos("LS", "Tracking")
    l_v = cv2.getTrackbarPos("LV", "Tracking")

    u_h = cv2.getTrackbarPos("UH", "Tracking")
    u_s = cv2.getTrackbarPos("US", "Tracking")
    u_v = cv2.getTrackbarPos("UV", "Tracking")

    l_b = np.array([l_h, l_s, l_v])
    u_b = np.array([u_h, u_s, u_v])

    mask = cv2.inRange(hsv, l_b, u_b)
    res = cv2.bitwise_and(frame, frame, mask=mask)

    # Finn gjennomsnittlig HSV verdi i masken for å detektere hovedfargen
    mean_hue = cv2.mean(hsv, mask=mask)[0]  # Gjennomsnittlig hue (H)
    mean_saturation = cv2.mean(hsv, mask=mask)[1]  # Gjennomsnittlig saturation (S)
    mean_value = cv2.mean(hsv, mask=mask)[2]  # Gjennomsnittlig value (V)

    detected_color = detect_color(mean_hue, mean_saturation, mean_value)
    print("Fargen som sees er:", detected_color)

    cv2.imshow("live transmission", frame)
    cv2.imshow("mask", mask)
    cv2.imshow("res", res)

    key = cv2.waitKey(5)
    if key == ord('q'):
        break

    time.sleep(0.05)  # Juster oppdateringsfrekvensen om nødvendig

cv2.destroyAllWindows()
