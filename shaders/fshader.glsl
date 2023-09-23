#version 330 core

// 给光源数据一个结构体
struct Light{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 position;

    // 光源衰减系数的三个参�?    
    float constant; // 常数�?    
    float linear;	// 一次项
    float quadratic;// 二次�?
};

// 给物体材质数据一个结构体
struct Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float shininess;
};

in vec3 position;
in vec3 normal;
in vec2 texCoord;


// 相机坐标
uniform vec3 eye_position;
// 光源
uniform Light light;
// 物体材质
uniform Material material;

uniform int isShadow;
uniform int ambientOpen;
uniform int DiffuseOpen;
uniform int SpecularOpen;
// 纹理数据
uniform sampler2D texture1;


out vec4 fColor;

void main()
{
	if (isShadow == 1) {
		fColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {
		// 将顶点坐标、光源坐标和法向量转换到相机坐标系
		vec3 norm=(vec4(normal,0.)).xyz;
		vec3 pos=vec3(position).xyz;
		vec3 l_pos=(vec4(light.position,1.)).xyz;
		
		vec3 N=normalize(norm);// 曲面法向量向量 n = normalize( vertex_normal )
		vec3 V=normalize(-eye_position);// 视线方向v = normalize( eye_position - vertex_position )
		vec3 L=normalize(l_pos-pos);// 入射方向 l = normalize( light_position - vertex_position )
		vec3 R=reflect(L,N);// 反射方向 r = reflect( -l, n )
		
		// 环境光分量I_a
		vec4 I_a=vec4(0.,0.,0.,1.);
		if(ambientOpen!=1)
		I_a=light.ambient*material.ambient;
		
		// 计算漫反射系数alpha和漫反射分量I_d
		float diffuse_dot=0.;
		if(DiffuseOpen!=1){
			diffuse_dot=max(dot(L,N),0.);
		}
		vec4 I_d=diffuse_dot*light.diffuse*material.diffuse;
		
		// 计算高光系数beta和镜面反射分量I_s
		float specular_dot_pow=0.;
		if(SpecularOpen!=1){
			specular_dot_pow=pow(max(dot(R,V),0.),material.shininess);
		}
		vec4 I_s=specular_dot_pow*light.specular*material.specular;
		
		// 注意如果光源在背面则去除高光
		if(dot(L,N)<0.){
			I_s=vec4(0.,0.,0.,1.);
		}
		
		// 合并三个分量的颜色，修正透明度
		vec4 mcolor = I_a + I_d + I_s;
		mcolor.a = 1.0;
		fColor = texture( texture1, texCoord )*0.8 + mcolor*0.2;
		// 合并三个分量的颜色，修正透明度
		//fColor=I_a+I_d+I_s;
		//fColor.a=1.;
		// fColor = texture2D( texture1, texCoord );
		//fColor = texture( texture1, texCoord );
	}
}
