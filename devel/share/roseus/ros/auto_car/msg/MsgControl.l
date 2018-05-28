;; Auto-generated. Do not edit!


(when (boundp 'auto_car::MsgControl)
  (if (not (find-package "AUTO_CAR"))
    (make-package "AUTO_CAR"))
  (shadow 'MsgControl (find-package "AUTO_CAR")))
(unless (find-package "AUTO_CAR::MSGCONTROL")
  (make-package "AUTO_CAR::MSGCONTROL"))

(in-package "ROS")
;;//! \htmlinclude MsgControl.msg.html


(defclass auto_car::MsgControl
  :super ros::object
  :slots (_data ))

(defmethod auto_car::MsgControl
  (:init
   (&key
    ((:data __data) 0)
    )
   (send-super :init)
   (setq _data (round __data))
   self)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:serialization-length
   ()
   (+
    ;; int32 _data
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _data
       (write-long _data s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _data
     (setq _data (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get auto_car::MsgControl :md5sum-) "da5909fbe378aeaf85e547e830cc1bb7")
(setf (get auto_car::MsgControl :datatype-) "auto_car/MsgControl")
(setf (get auto_car::MsgControl :definition-)
      "int32 data

")



(provide :auto_car/MsgControl "da5909fbe378aeaf85e547e830cc1bb7")


