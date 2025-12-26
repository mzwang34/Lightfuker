import os
import glob
import numpy as np
import xml.etree.ElementTree as ET
from xml.dom import minidom

OUTPUT_DIR = "disney_total"
STEPS = np.linspace(0.0, 1.0, 11)

def prettify_xml(elem):
    rough_string = ET.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="    ")

def get_param_name_from_filename(filename):
    base = os.path.basename(filename)
    name_no_ext = os.path.splitext(base)[0]
    
    if name_no_ext.startswith("disney_"):
        return name_no_ext.replace("disney_", "")
    return None

def process_files():
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    source_files = glob.glob("disney_*.xml")
    
    if not source_files:
        print("failed to find files!")
        return

    for xml_file in source_files:
        param_name = get_param_name_from_filename(xml_file)
        if not param_name:
            continue
        
        try:
            tree = ET.parse(xml_file)
            root = tree.getroot()
        except ET.ParseError:
            continue

        scene = root.find("scene")
        if scene is None: continue
        instance = scene.find("instance")
        if instance is None: continue
        bsdf = instance.find("bsdf")
        if bsdf is None: 
            continue

        for light in scene.findall("light"):
            if light.get("type") == "envmap":
                texture = light.find("texture")
                if texture is not None and texture.get("type") == "image":
                    old_path = "../../textures/kloofendal_overcast_1k.hdr"
                    new_path = "../../../textures/kloofendal_overcast_1k.hdr"
                    if texture.get("filename") == old_path:
                        texture.set("filename", new_path)

        image_node = root.find("image")

        for val in STEPS:
            val_str = f"{val:.1f}" 

            new_id = f"disney_{param_name}_{val_str}"
            new_filename = f"{new_id}.xml"

            if image_node is not None:
                image_node.set("id", new_id)

            target_node = bsdf.find(f"./float[@name='{param_name}']")
            target_node.set("value", f"{val:.1f}")

            output_path = os.path.join(OUTPUT_DIR, new_filename)
            raw_xml_str = prettify_xml(root)

            clean_lines = []
            for line in raw_xml_str.split('\n'):
                stripped = line.strip()
                if not stripped: 
                    continue
                if stripped.startswith("<?xml"): 
                    continue
                
                clean_lines.append(line)
            
            final_xml_str = "\n".join(clean_lines)
            
            with open(output_path, "w", encoding="utf-8") as f:
                f.write(final_xml_str)

if __name__ == "__main__":
    process_files()