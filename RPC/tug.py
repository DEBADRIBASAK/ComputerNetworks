import tensorflow as tf
x = tf.constant(5)
y = tf.constant(4)
z = tf.multiply(x,y)
sess = tf.Session()
print(sess.run(z,{}))
