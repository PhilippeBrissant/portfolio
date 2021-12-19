package app.android.holder;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.recyclerview.widget.RecyclerView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import app.android.R;

public class ImageHolder extends RecyclerView.ViewHolder {

    private ImageView imageView ;

    public ImageHolder(final View itemView) {
        super(itemView);
        imageView = (ImageView) itemView.findViewById(R.id.imageViewStreaming);
        final DatabaseReference database = FirebaseDatabase.getInstance().getReference().child("realtime/image");

        ValueEventListener listener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                String imagembase64 = dataSnapshot.getValue(String.class);
                Toast.makeText(itemView.getContext(), "Atualizou imagem", Toast.LENGTH_SHORT).show();

                exibeImage(imagembase64);
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                //tvTituloLuz.setText(databaseError.getMessage());

            }
        };

        database.addValueEventListener(listener);


//        imageView.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                String referencia = "configuracao/imagem-realtime";
//                if (MainActivity.valoresRealtime.get("imagem").equals("0"))
//                    database.getParent().getParent().child(referencia).setValue("1");
//                else{
//                    database.getParent().getParent().child(referencia).setValue("0");
//                    database.setValue("0");
//                    imageView.setImageDrawable(view.getContext().getDrawable(R.drawable.ic_launcher_background));
//                }
//            }
//        });
    }

    public void exibeImage(String base64Image){
        //convert to base64
//        Bitmap bmp =  BitmapFactory.decodeResource(getResources(), R.drawable.ic_launcher_background);//your image
//        ByteArrayOutputStream bYtE = new ByteArrayOutputStream();
//        bmp.compress(Bitmap.CompressFormat.PNG, 100, bYtE);
//        bmp.recycle();
//        byte[] byteArray = bYtE.toByteArray();
//        String imageFile = Base64.encodeToString(byteArray, Base64.DEFAULT);


        //decode from base64
        try {
            byte [] encodeByte= Base64.decode(base64Image,Base64.DEFAULT);
            Bitmap bitmap = BitmapFactory.decodeByteArray(encodeByte, 0, encodeByte.length);

            imageView.setImageBitmap(bitmap);
        } catch(Exception e) {
            e.getMessage();
        }
    }
}
