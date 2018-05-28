; Auto-generated. Do not edit!


(cl:in-package auto_car-msg)


;//! \htmlinclude MsgControl.msg.html

(cl:defclass <MsgControl> (roslisp-msg-protocol:ros-message)
  ((data
    :reader data
    :initarg :data
    :type cl:integer
    :initform 0))
)

(cl:defclass MsgControl (<MsgControl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <MsgControl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'MsgControl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name auto_car-msg:<MsgControl> is deprecated: use auto_car-msg:MsgControl instead.")))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <MsgControl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader auto_car-msg:data-val is deprecated.  Use auto_car-msg:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <MsgControl>) ostream)
  "Serializes a message object of type '<MsgControl>"
  (cl:let* ((signed (cl:slot-value msg 'data)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <MsgControl>) istream)
  "Deserializes a message object of type '<MsgControl>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'data) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<MsgControl>)))
  "Returns string type for a message object of type '<MsgControl>"
  "auto_car/MsgControl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'MsgControl)))
  "Returns string type for a message object of type 'MsgControl"
  "auto_car/MsgControl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<MsgControl>)))
  "Returns md5sum for a message object of type '<MsgControl>"
  "da5909fbe378aeaf85e547e830cc1bb7")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'MsgControl)))
  "Returns md5sum for a message object of type 'MsgControl"
  "da5909fbe378aeaf85e547e830cc1bb7")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<MsgControl>)))
  "Returns full string definition for message of type '<MsgControl>"
  (cl:format cl:nil "int32 data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'MsgControl)))
  "Returns full string definition for message of type 'MsgControl"
  (cl:format cl:nil "int32 data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <MsgControl>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <MsgControl>))
  "Converts a ROS message object to a list"
  (cl:list 'MsgControl
    (cl:cons ':data (data msg))
))
