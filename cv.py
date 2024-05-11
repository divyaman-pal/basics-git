import cv2
import numpy as np

# Function to preprocess the image
def preprocess_image(frame):
    # Convert the frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Apply GaussianBlur to reduce noise and improve accuracy
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)

    # Apply thresholding to create a binary image
    _, thresholded = cv2.threshold(blurred, 127, 255, cv2.THRESH_BINARY)

    return thresholded

# Function to detect hand gestures
def detect_hand_gesture(frame):
    # Preprocess the image
    processed_frame = preprocess_image(frame)

    # Find contours in the binary image
    contours, _ = cv2.findContours(processed_frame, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Check if any contours are found
    if contours:
        # Find the contour with the largest area
        largest_contour = max(contours, key=cv2.contourArea)

        # Calculate the convex hull of the largest contour
        hull = cv2.convexHull(largest_contour)

        # Draw the contour and convex hull on the original frame
        cv2.drawContours(frame, [largest_contour], 0, (0, 255, 0), 2)
        cv2.drawContours(frame, [hull], 0, (0, 0, 255), 3)

        # Calculate the area of the convex hull
        hull_area = cv2.contourArea(hull)

        # Check if the hand is open or closed based on the area of the convex hull
        if hull_area > 1000:
            return "Open Hand"
        else:
            return "Closed Hand"

    return "No Hand Detected"

# Capture video from the webcam
cap = cv2.VideoCapture(0)

while True:
    # Read a frame from the webcam
    ret, frame = cap.read()

    # Flip the frame horizontally for a later selfie-view display
    frame = cv2.flip(frame, 1)

    # Detect hand gesture
    gesture = detect_hand_gesture(frame)

    # Display the gesture on the frame
    cv2.putText(frame, f"Gesture: {gesture}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

    # Display the frame
    cv2.imshow('Hand Gesture Detection', frame)

    # Break the loop if 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the webcam and close all windows
cap.release()
cv2.destroyAllWindows()
